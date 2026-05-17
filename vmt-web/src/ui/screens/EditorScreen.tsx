import { useCallback, useMemo, useRef, useState } from 'react';
import { useNavigate } from 'react-router-dom';
import {
  addMachine,
  addTransition,
  exportPlantUml,
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
import { ComplexPlacementPanel } from '../widgets/ComplexPlacementPanel';
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
  const [complexName, setComplexName] = useState('Submachine');
  const canvasWrapRef = useRef<HTMLDivElement>(null);
  const canvasSize = useElementSize(canvasWrapRef, 200, 120);
  const undoStack = useRef<Project[]>([]);
  const [undoDepth, setUndoDepth] = useState(0);

  const rootBody = useMemo(() => getRootBody(project), [project]);
  const puml = useMemo(() => exportPlantUml(project), [project]);

  const selectedMachine = useMemo(() => {
    if (selection?.kind !== 'machine') return null;
    return findMachine(rootBody, selection.id) ?? null;
  }, [rootBody, selection]);

  const selectedTransition = useMemo(() => {
    if (selection?.kind !== 'transition') return null;
    return rootBody.transitions.find((t) => t.id === selection.id) ?? null;
  }, [rootBody, selection]);

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
    if (next === 'complex') setComplexName('Submachine');
  }, []);

  const handleMachineClick = useCallback(
    (machineId: string) => {
      const machine = findMachine(rootBody, machineId);
      if (!machine) return;

      if (tool === 'delete') {
        applyProject(removeMachine(project, rootBody.id, machineId));
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
            rootBody.id,
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
    [applyProject, linkDraft, project, rootBody, tool],
  );

  const handleTransitionClick = useCallback(
    (transitionId: string) => {
      if (tool === 'delete') {
        applyProject(removeTransition(project, rootBody.id, transitionId));
        setSelection(null);
        return;
      }
      if (tool === 'pointer' || tool === 'link') {
        setSelection({ kind: 'transition', id: transitionId });
      }
    },
    [applyProject, project, rootBody.id, tool],
  );

  const handleCanvasClick = useCallback(
    (point: Point) => {
      const snapped = snapToGrid(point);
      const machineType = machineTypeForTool(tool);
      if (machineType) {
        if (machineType === 'complex' && !complexName.trim()) return;
        applyProject(
          addMachine(project, rootBody.id, machineType, snapped, {
            complexName:
              machineType === 'complex' ? complexName.trim() : undefined,
          }),
        );
      }
    },
    [applyProject, complexName, project, rootBody.id, tool],
  );

  const handleMachineMoved = useCallback(
    (machineId: string, center: Point) => {
      applyProject(
        moveMachine(project, rootBody.id, machineId, snapToGrid(center)),
      );
    },
    [applyProject, project, rootBody.id],
  );

  const handleTransitionLabelMoved = useCallback(
    (transitionId: string, anchor: Point) => {
      const t = rootBody.transitions.find((x) => x.id === transitionId);
      if (!t) return;
      const routed = routeAllTransitions(
        rootBody.machines,
        rootBody.transitions,
        CELL,
      );
      const paths = rootBody.transitions.map((tr) => ({
        id: tr.id,
        points: routed.get(tr.id) ?? [],
      }));
      const path = paths.find((p) => p.id === transitionId)?.points ?? [];
      const projected = projectPointOnPolyline(anchor, path);
      applyProject(
        updateTransitionLabelT(
          project,
          rootBody.id,
          transitionId,
          projected.t,
        ),
      );
    },
    [applyProject, project, rootBody],
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
        updateWriteMachineSign(project, rootBody.id, selection.id, sign),
      );
    },
    [applyProject, project, rootBody.id, selection],
  );

  const handleTransitionConditions = useCallback(
    (conditions: TransitionConditions) => {
      if (selection?.kind !== 'transition') return;
      applyProject(
        updateTransitionConditions(
          project,
          rootBody.id,
          selection.id,
          conditions,
        ),
      );
    },
    [applyProject, project, rootBody.id, selection],
  );

  const handleExportPng = useCallback(() => {
    alert('PNG export: use browser screenshot or upcoming OffscreenCanvas export.');
  }, []);

  return (
    <div className="screen editor-screen">
      <div className="editor-layout">
        <EditorToolbar activeTool={tool} onToolChange={handleToolChange} />
        <div className="editor-main">
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
              />
              {tool === 'complex' ? (
                <ComplexPlacementPanel
                  name={complexName}
                  onNameChange={setComplexName}
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
