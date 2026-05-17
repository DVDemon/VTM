import { useCallback, useMemo, useRef, useState } from 'react';
import { useNavigate } from 'react-router-dom';
import {
  addMachine,
  addTransition,
  createSubmachine,
  exportPlantUml,
  findBody,
  findMachine,
  getRootBody,
  machineAcceptsIncoming,
  machineAcceptsOutgoing,
  moveMachine,
  projectPointOnPolyline,
  removeMachine,
  removeTransition,
  routeAllTransitions,
  updateTransitionConditions,
  updateTransitionLabelT,
  updateWriteMachineSign,
  type Point,
  type Project,
  type TransitionConditions,
} from '@core/index';
import { machineTypeForTool } from '../widgets/editorTools';
import { useProject } from '../../storage/ProjectContext';
import { exportPlantUmlFile } from '../files/projectFiles';
import {
  DiagramStage,
  type EditorSelection,
  type LinkDraft,
} from '../canvas/DiagramStage';
import { EditorActionBar } from '../widgets/EditorActionBar';
import { EditorToolbar } from '../widgets/EditorToolbar';
import { EditorInspector } from '../widgets/EditorInspector';
import { BodyBreadcrumb } from '../widgets/BodyBreadcrumb';
import {
  SubmachinePanel,
  type SubmachinePlaceMode,
} from '../widgets/SubmachinePanel';
import { useElementSize } from '../hooks/useElementSize';
import type { EditorTool } from '../widgets/editorTools';

const CELL = 48;

function snapToGrid(point: Point): Point {
  return {
    x: Math.round(point.x / CELL) * CELL,
    y: Math.round(point.y / CELL) * CELL,
  };
}

export function EditorScreen() {
  const navigate = useNavigate();
  const { project, setProject } = useProject();
  const [tool, setTool] = useState<EditorTool>('pointer');
  const [selection, setSelection] = useState<EditorSelection>(null);
  const [zoom, setZoom] = useState(100);
  const [viewOffset, setViewOffset] = useState<Point>({ x: 0, y: 0 });
  const [linkDraft, setLinkDraft] = useState<LinkDraft | null>(null);
  const [editingBodyId, setEditingBodyId] = useState<string | null>(null);
  const [complexName, setComplexName] = useState('Submachine');
  const [submachineMode, setSubmachineMode] =
    useState<SubmachinePlaceMode>('new');
  const [selectedInnerBodyId, setSelectedInnerBodyId] = useState('');
  const canvasWrapRef = useRef<HTMLDivElement>(null);
  const canvasSize = useElementSize(canvasWrapRef, 200, 120);
  const undoStack = useRef<Project[]>([]);
  const [undoDepth, setUndoDepth] = useState(0);

  const rootBody = useMemo(() => getRootBody(project), [project]);
  const activeBody = useMemo(() => {
    if (editingBodyId) {
      return findBody(project, editingBodyId) ?? rootBody;
    }
    return rootBody;
  }, [project, editingBodyId, rootBody]);

  const puml = useMemo(() => exportPlantUml(project), [project]);

  const selectedMachine = useMemo(() => {
    if (selection?.kind !== 'machine') return null;
    return findMachine(activeBody, selection.id) ?? null;
  }, [activeBody, selection]);

  const selectedTransition = useMemo(() => {
    if (selection?.kind !== 'transition') return null;
    return activeBody.transitions.find((t) => t.id === selection.id) ?? null;
  }, [activeBody, selection]);

  const pushUndo = useCallback(() => {
    undoStack.current.push(project);
    if (undoStack.current.length > 50) undoStack.current.shift();
    setUndoDepth(undoStack.current.length);
  }, [project]);

  const applyProject = useCallback(
    (next: Project) => {
      pushUndo();
      setProject(next);
    },
    [pushUndo, setProject],
  );

  const handleUndo = useCallback(() => {
    const prev = undoStack.current.pop();
    setUndoDepth(undoStack.current.length);
    if (prev) setProject(prev);
  }, [setProject]);

  const handleToolChange = useCallback((next: EditorTool) => {
    setTool(next);
    setLinkDraft(null);
    if (next !== 'pointer') setSelection(null);
    if (next === 'complex') {
      setComplexName('Submachine');
      setSubmachineMode('new');
      setSelectedInnerBodyId('');
    }
  }, []);

  const handleMachineClick = useCallback(
    (machineId: string) => {
      const machine = findMachine(activeBody, machineId);
      if (!machine) return;

      if (tool === 'delete') {
        applyProject(removeMachine(project, activeBody.id, machineId));
        setSelection(null);
        return;
      }

      if (tool === 'link') {
        if (!linkDraft) {
          if (!machineAcceptsOutgoing(machine)) return;
          const out = machine;
          setLinkDraft({
            startMachineId: machineId,
            cursor: out.center,
          });
          setSelection({ kind: 'machine', id: machineId });
          return;
        }

        if (machineId === linkDraft.startMachineId) return;
        if (!machineAcceptsIncoming(machine)) return;
        applyProject(
          addTransition(
            project,
            activeBody.id,
            linkDraft.startMachineId,
            machineId,
          ),
        );
        setLinkDraft(null);
        setSelection(null);
        return;
      }

      setSelection({ kind: 'machine', id: machineId });
    },
    [activeBody, applyProject, linkDraft, project, tool],
  );

  const handleTransitionClick = useCallback(
    (transitionId: string) => {
      if (tool === 'delete') {
        applyProject(removeTransition(project, activeBody.id, transitionId));
        setSelection(null);
        return;
      }
      if (tool === 'pointer' || tool === 'link') {
        setSelection({ kind: 'transition', id: transitionId });
      }
    },
    [activeBody.id, applyProject, project, tool],
  );

  const handleCanvasClick = useCallback(
    (point: Point) => {
      const snapped = snapToGrid(point);
      const machineType = machineTypeForTool(tool);
      if (machineType) {
        if (machineType === 'complex') {
          if (submachineMode === 'new' && !complexName.trim()) return;
          if (submachineMode === 'existing' && !selectedInnerBodyId) return;
        }
        applyProject(
          addMachine(project, activeBody.id, machineType, snapped, {
            complexName:
              machineType === 'complex' && submachineMode === 'new'
                ? complexName.trim()
                : undefined,
            innerBodyId:
              machineType === 'complex' && submachineMode === 'existing'
                ? selectedInnerBodyId
                : undefined,
          }),
        );
      }
    },
    [
      activeBody.id,
      applyProject,
      complexName,
      project,
      selectedInnerBodyId,
      submachineMode,
      tool,
    ],
  );

  const handleMachineMoved = useCallback(
    (machineId: string, center: Point) => {
      applyProject(
        moveMachine(project, activeBody.id, machineId, snapToGrid(center)),
      );
    },
    [activeBody.id, applyProject, project],
  );

  const handleTransitionLabelMoved = useCallback(
    (transitionId: string, anchor: Point) => {
      const t = activeBody.transitions.find((x) => x.id === transitionId);
      if (!t) return;
      const routed = routeAllTransitions(
        activeBody.machines,
        activeBody.transitions,
        CELL,
      );
      const paths = activeBody.transitions.map((tr) => ({
        id: tr.id,
        points: routed.get(tr.id) ?? [],
      }));
      const path = paths.find((p) => p.id === transitionId)?.points ?? [];
      const projected = projectPointOnPolyline(anchor, path);
      applyProject(
        updateTransitionLabelT(
          project,
          activeBody.id,
          transitionId,
          projected.t,
        ),
      );
    },
    [activeBody, applyProject, project],
  );

  const handleLinkCursorMove = useCallback(
    (cursor: Point, hoverMachineId?: string) => {
      setLinkDraft((draft) =>
        draft ? { ...draft, cursor, hoverMachineId } : draft,
      );
    },
    [],
  );

  const handleWriteSign = useCallback(
    (sign: string) => {
      if (selection?.kind !== 'machine') return;
      applyProject(
        updateWriteMachineSign(project, activeBody.id, selection.id, sign),
      );
    },
    [activeBody.id, applyProject, project, selection],
  );

  const handleTransitionConditions = useCallback(
    (conditions: TransitionConditions) => {
      if (selection?.kind !== 'transition') return;
      applyProject(
        updateTransitionConditions(
          project,
          activeBody.id,
          selection.id,
          conditions,
        ),
      );
    },
    [activeBody.id, applyProject, project, selection],
  );

  const handleNavigateBody = useCallback((bodyId: string) => {
    setEditingBodyId(bodyId);
    setSelection(null);
    setLinkDraft(null);
  }, []);

  const handleComplexDoubleClick = useCallback(
    (_machineId: string, innerBodyId: string) => {
      handleNavigateBody(innerBodyId);
    },
    [handleNavigateBody],
  );

  const handleCreateSubmachine = useCallback(() => {
    const name = complexName.trim() || 'Submachine';
    try {
      const next = createSubmachine(project, name);
      applyProject(next);
      const created = next.bodies.find((b) => b.name === name);
      if (created) {
        setSelectedInnerBodyId(created.id);
        setSubmachineMode('existing');
        handleNavigateBody(created.id);
      }
    } catch (e) {
      alert(e instanceof Error ? e.message : String(e));
    }
  }, [applyProject, complexName, handleNavigateBody, project]);

  const handleExportPng = useCallback(() => {
    alert('PNG export: use browser screenshot or upcoming OffscreenCanvas export.');
  }, []);

  return (
    <div className="screen editor-screen">
      <div className="editor-layout">
        <EditorToolbar activeTool={tool} onToolChange={handleToolChange} />
        <div className="editor-main">
          <BodyBreadcrumb
            project={project}
            editingBodyId={activeBody.id}
            onNavigate={handleNavigateBody}
          />
          <EditorActionBar
            zoom={zoom}
            canUndo={undoDepth > 0}
            onUndo={handleUndo}
            onExportPng={handleExportPng}
            onExportPuml={() => exportPlantUmlFile(puml, project.name)}
            onRun={() => navigate('/compiler')}
            onExport4th={() => navigate('/export-4th')}
            onZoomChange={setZoom}
          />
          <div className="editor-workspace">
            <div ref={canvasWrapRef} className="editor-canvas-wrap">
              <DiagramStage
                project={project}
                bodyId={activeBody.id}
                width={canvasSize.width}
                height={canvasSize.height}
                zoom={zoom}
                tool={tool}
                selection={selection}
                viewOffset={viewOffset}
                linkDraft={linkDraft}
                onSelectionChange={setSelection}
                onViewOffsetChange={setViewOffset}
                onLinkCursorMove={handleLinkCursorMove}
                onMachineClick={handleMachineClick}
                onTransitionClick={handleTransitionClick}
                onMachineMoved={handleMachineMoved}
                onTransitionLabelMoved={handleTransitionLabelMoved}
                onCanvasClick={handleCanvasClick}
                onComplexDoubleClick={handleComplexDoubleClick}
              />
              {tool === 'complex' ? (
                <SubmachinePanel
                  project={project}
                  currentBodyId={activeBody.id}
                  mode={submachineMode}
                  onModeChange={setSubmachineMode}
                  newName={complexName}
                  onNewNameChange={setComplexName}
                  selectedInnerBodyId={selectedInnerBodyId}
                  onSelectInnerBodyId={setSelectedInnerBodyId}
                  onCreateSubmachine={handleCreateSubmachine}
                />
              ) : (
                <EditorInspector
                  project={project}
                  selectedMachine={selectedMachine}
                  selectedTransition={selectedTransition}
                  onWriteSign={handleWriteSign}
                  onTransitionConditions={handleTransitionConditions}
                />
              )}
            </div>
          </div>
        </div>
      </div>
    </div>
  );
}
