import { useMemo, useRef, useState } from 'react';
import type Konva from 'konva';
import { Group, Layer, Line, Rect, Shape, Stage, Text } from 'react-konva';
import {
  buildConnectorPath,
  flattenPath,
  getRootBody,
  isPointNearStroke,
  machineAcceptsIncoming,
  machineDisplayLabel,
  machineLabelFontSize,
  resolveConditionsAnchor,
  formatTransitionConditions,
  routeAllTransitions,
  computeWorldSurface,
  gridLinesForBounds,
  snapPortHeights,
  inputPoint,
  machineRect,
  outputPoint,
  type Machine,
  type Point,
  type Project,
} from '@core/index';
import { vmtTheme } from '../theme/vmtTheme';
import type { EditorTool } from '../widgets/editorTools';

const CELL = 48;
const PAN_DRAG_THRESHOLD = 4;

export type EditorSelection =
  | { kind: 'machine'; id: string }
  | { kind: 'transition'; id: string }
  | null;

export interface LinkDraft {
  readonly startMachineId: string;
  readonly cursor: Point;
  readonly hoverMachineId?: string;
}

export type DiagramInteractionMode = 'edit' | 'navigate';

interface DiagramStageProps {
  project: Project;
  width: number;
  height: number;
  zoom?: number;
  /** navigate — только просмотр и pan (дебаггер); edit — редактор */
  interactionMode?: DiagramInteractionMode;
  tool?: EditorTool;
  selection?: EditorSelection;
  viewOffset?: Point;
  linkDraft?: LinkDraft | null;
  onSelectionChange?: (selection: EditorSelection) => void;
  onViewOffsetChange?: (offset: Point) => void;
  onLinkCursorMove?: (cursor: Point, hoverMachineId?: string) => void;
  onMachineClick?: (machineId: string) => void;
  onTransitionClick?: (transitionId: string) => void;
  onMachineMoved?: (machineId: string, center: Point) => void;
  onTransitionLabelMoved?: (transitionId: string, anchor: Point) => void;
  onCanvasClick?: (point: Point) => void;
  /** @deprecated use selection */
  selectedMachineId?: string | null;
}

function applyMachineDrag(
  machines: readonly Machine[],
  drag: { readonly machineId: string; readonly center: Point } | null,
): readonly Machine[] {
  if (!drag) return machines;
  return machines.map((m) =>
    m.id === drag.machineId ? { ...m, center: drag.center } : m,
  );
}

function hitMachineAt(machines: readonly Machine[], point: Point): boolean {
  return machines.some((m) => {
    const r = machineRect(m);
    return (
      point.x >= r.x &&
      point.x <= r.x + r.width &&
      point.y >= r.y &&
      point.y <= r.y + r.height
    );
  });
}

export function DiagramStage({
  project,
  width,
  height,
  zoom = 100,
  interactionMode = 'edit',
  tool = 'pointer',
  selection: selectionProp,
  viewOffset: viewOffsetProp,
  linkDraft = null,
  onSelectionChange = () => {},
  onViewOffsetChange = () => {},
  onLinkCursorMove = () => {},
  onMachineClick = () => {},
  onTransitionClick = () => {},
  onMachineMoved,
  onTransitionLabelMoved,
  onCanvasClick,
  selectedMachineId,
}: DiagramStageProps) {
  const selection: EditorSelection =
    selectionProp ??
    (selectedMachineId ? { kind: 'machine', id: selectedMachineId } : null);
  const viewOffset = viewOffsetProp ?? { x: 0, y: 0 };
  const stageRef = useRef<Konva.Stage>(null);
  const panStart = useRef<{
    screen: Point;
    offset: Point;
    pendingClick: boolean;
  } | null>(null);
  const [panning, setPanning] = useState(false);
  const [machineDrag, setMachineDrag] = useState<{
    machineId: string;
    center: Point;
  } | null>(null);
  const body = getRootBody(project);
  const alphabet = body.alphabet || project.alphabet;
  const scale = zoom / 100;
  const panOnly = interactionMode === 'navigate' || tool === 'pan';
  const nodesDraggable =
    interactionMode === 'edit' && tool === 'pointer' && !panning;

  const layoutMachines = useMemo(
    () => applyMachineDrag(body.machines, machineDrag),
    [body.machines, machineDrag],
  );

  const { transitionPaths, worldSurface, gridLines } = useMemo(() => {
    const routed = routeAllTransitions(layoutMachines, body.transitions, CELL);
    const paths = body.transitions.map((t) => {
      const points = routed.get(t.id) ?? [];
      const segments = buildConnectorPath(snapPortHeights(points));
      return { transition: t, points, segments };
    });
    const routedPaths = paths.map((p) => p.points).filter((path) => path.length >= 2);
    const surface = computeWorldSurface(
      layoutMachines,
      routedPaths,
      viewOffset,
      width,
      height,
    );
    return {
      transitionPaths: paths,
      worldSurface: surface,
      gridLines: gridLinesForBounds(surface, CELL),
    };
  }, [layoutMachines, body.transitions, viewOffset, width, height]);

  const linkPreviewPoints = useMemo(() => {
    if (!linkDraft) return null;
    const start = body.machines.find((m) => m.id === linkDraft.startMachineId);
    if (!start) return null;
    const out = outputPoint(machineRect(start));
    if (linkDraft.hoverMachineId) {
      const end = body.machines.find((m) => m.id === linkDraft.hoverMachineId);
      if (end && machineAcceptsIncoming(end)) {
        const fin = inputPoint(machineRect(end));
        return [out, fin];
      }
    }
    return [out, linkDraft.cursor];
  }, [body.machines, linkDraft]);

  const pointerToCanvas = (stage: Konva.Stage): Point | null => {
    const pos = stage.getPointerPosition();
    if (!pos) return null;
    return {
      x: pos.x / scale - viewOffset.x,
      y: pos.y / scale - viewOffset.y,
    };
  };

  const findTransitionAt = (point: Point): string | null => {
    for (const { transition, segments } of transitionPaths) {
      if (isPointNearStroke(segments, point, 8)) return transition.id;
    }
    return null;
  };

  const beginPan = (stage: Konva.Stage, pendingClick = false) => {
    const pos = stage.getPointerPosition();
    if (!pos) return;
    panStart.current = {
      screen: { x: pos.x, y: pos.y },
      offset: { ...viewOffset },
      pendingClick,
    };
    setPanning(!pendingClick);
  };

  const applyPan = (stage: Konva.Stage) => {
    const start = panStart.current;
    if (!start) return;
    const pos = stage.getPointerPosition();
    if (!pos) return;
    onViewOffsetChange({
      x: start.offset.x + (pos.x - start.screen.x) / scale,
      y: start.offset.y + (pos.y - start.screen.y) / scale,
    });
  };

  const cursorStyle = panOnly
    ? panning
      ? 'grabbing'
      : 'grab'
    : tool === 'delete'
      ? 'not-allowed'
      : 'default';

  return (
    <Stage
      ref={stageRef}
      width={width}
      height={height}
      style={{ cursor: cursorStyle, display: 'block' }}
      onMouseDown={(e) => {
        const stage = e.target.getStage()!;
        const point = pointerToCanvas(stage);
        if (!point) return;

        const isMiddleButton = e.evt.button === 1;
        const clickedOnCanvas =
          e.target === stage || e.target.getClassName() === 'Layer';

        if (panOnly || isMiddleButton) {
          e.evt.preventDefault();
          beginPan(stage);
          return;
        }

        if (
          clickedOnCanvas &&
          tool === 'pointer' &&
          !hitMachineAt(layoutMachines, point)
        ) {
          beginPan(stage, true);
          return;
        }

        if (clickedOnCanvas) {
          const tid = findTransitionAt(point);
          if (tid) {
            onTransitionClick(tid);
            return;
          }
          onSelectionChange(null);
          onCanvasClick?.(point);
        }
      }}
      onMouseMove={(e) => {
        const stage = e.target.getStage()!;
        const point = pointerToCanvas(stage);
        if (!point) return;

        if (panStart.current) {
          const pos = stage.getPointerPosition();
          if (pos) {
            const dx = pos.x - panStart.current.screen.x;
            const dy = pos.y - panStart.current.screen.y;
            if (
              panning ||
              Math.hypot(dx, dy) >= PAN_DRAG_THRESHOLD ||
              panOnly
            ) {
              if (!panning) setPanning(true);
              applyPan(stage);
            }
          }
          if (panning) return;
        }

        if (tool === 'link' && linkDraft) {
          let hoverId: string | undefined;
          for (const m of body.machines) {
            const r = machineRect(m);
            if (
              point.x >= r.x &&
              point.x <= r.x + r.width &&
              point.y >= r.y &&
              point.y <= r.y + r.height
            ) {
              hoverId = m.id;
              break;
            }
          }
          onLinkCursorMove(point, hoverId);
        }
      }}
      onMouseUp={(e) => {
        const stage = e.target.getStage()!;
        const start = panStart.current;
        if (start?.pendingClick && !panning && pointerToCanvas(stage)) {
          const point = pointerToCanvas(stage)!;
          const tid = findTransitionAt(point);
          if (tid) {
            onTransitionClick(tid);
          } else {
            onSelectionChange(null);
            onCanvasClick?.(point);
          }
        }
        panStart.current = null;
        setPanning(false);
      }}
      onMouseLeave={() => {
        panStart.current = null;
        setPanning(false);
      }}
    >
      <Layer scaleX={scale} scaleY={scale}>
        <Group x={viewOffset.x} y={viewOffset.y}>
          <Rect
            x={worldSurface.x}
            y={worldSurface.y}
            width={worldSurface.width}
            height={worldSurface.height}
            fill={vmtTheme.diagramBackground}
            listening={false}
          />
          <Line
            points={gridLines}
            stroke={vmtTheme.diagramGrid}
            strokeWidth={1}
            dash={[2, 4]}
            listening={false}
          />

          {transitionPaths.map(({ transition, points, segments }) =>
            points.length >= 2 ? (
              <Shape
                key={transition.id}
                stroke={
                  selection?.kind === 'transition' &&
                  selection.id === transition.id
                    ? '#1976D2'
                    : vmtTheme.diagramLine
                }
                strokeWidth={
                  selection?.kind === 'transition' &&
                  selection.id === transition.id
                    ? 3
                    : 2
                }
                sceneFunc={(ctx, shape) => {
                  if (segments.length === 0) return;
                  ctx.beginPath();
                  ctx.moveTo(segments[0]!.p0.x, segments[0]!.p0.y);
                  for (const seg of segments) {
                    ctx.bezierCurveTo(
                      seg.c1.x,
                      seg.c1.y,
                      seg.c2.x,
                      seg.c2.y,
                      seg.p3.x,
                      seg.p3.y,
                    );
                  }
                  ctx.strokeShape(shape);
                }}
              />
            ) : null,
          )}
          {linkPreviewPoints && linkPreviewPoints.length >= 2 ? (
            <Line
              points={linkPreviewPoints.flatMap((p) => [p.x, p.y])}
              stroke="#1976D2"
              strokeWidth={2}
              dash={[6, 6]}
              listening={false}
            />
          ) : null}

          {transitionPaths.map(({ transition, points }) => {
            const label = formatTransitionConditions(
              alphabet,
              transition.conditions,
            );
            const anchor = resolveConditionsAnchor(transition, points);
            const selected =
              selection?.kind === 'transition' &&
              selection.id === transition.id;
            const flat = flattenPath(
              buildConnectorPath(snapPortHeights(points)),
              8,
            );
            return (
              <Group key={`${transition.id}-label`}>
                <Shape
                  onClick={(e) => {
                    e.cancelBubble = true;
                    onTransitionClick(transition.id);
                  }}
                  sceneFunc={(ctx, shape) => {
                    if (flat.length < 2) return;
                    ctx.beginPath();
                    ctx.moveTo(flat[0]!.x, flat[0]!.y);
                    for (let i = 1; i < flat.length; i++) {
                      ctx.lineTo(flat[i]!.x, flat[i]!.y);
                    }
                    ctx.strokeShape(shape);
                  }}
                  stroke="transparent"
                  strokeWidth={12}
                />
                <Text
                  x={anchor.x - 16}
                  y={anchor.y - 10}
                  width={32}
                  align="center"
                  text={label}
                  fontSize={13}
                  fontStyle="bold"
                  fill={selected ? '#1565C0' : vmtTheme.textOnLight}
                  draggable={nodesDraggable && selected}
                  onClick={(e) => {
                    e.cancelBubble = true;
                    onTransitionClick(transition.id);
                  }}
                  onDragEnd={(e) => {
                    const node = e.target;
                    onTransitionLabelMoved?.(transition.id, {
                      x: node.x() + 16,
                      y: node.y() + 10,
                    });
                  }}
                />
              </Group>
            );
          })}

          {layoutMachines.map((m) => {
            const label = machineDisplayLabel(m, alphabet);
            const fontSize = machineLabelFontSize(m);
            const pad = 4;
            const hw = m.size.x / 2;
            const hh = m.size.y / 2;
            const selected =
              selection?.kind === 'machine' && selection.id === m.id;
            const linkHover =
              linkDraft?.hoverMachineId === m.id ||
              linkDraft?.startMachineId === m.id;
            return (
              <Group
                key={m.id}
                x={m.center.x}
                y={m.center.y}
                draggable={nodesDraggable}
                onClick={(e) => {
                  if (panOnly) return;
                  e.cancelBubble = true;
                  onMachineClick(m.id);
                }}
                onDragStart={(e) => {
                  if (!nodesDraggable) {
                    e.target.stopDrag();
                    return;
                  }
                  setMachineDrag({
                    machineId: m.id,
                    center: { ...m.center },
                  });
                }}
                onDragMove={(e) => {
                  const node = e.target;
                  setMachineDrag({
                    machineId: m.id,
                    center: { x: node.x(), y: node.y() },
                  });
                }}
                onDragEnd={(e) => {
                  setMachineDrag(null);
                  const node = e.target;
                  onMachineMoved?.(m.id, {
                    x: node.x(),
                    y: node.y(),
                  });
                }}
              >
                <Rect
                  x={-hw}
                  y={-hh}
                  width={m.size.x}
                  height={m.size.y}
                  fill={
                    linkHover
                      ? '#E3F2FD'
                      : selected
                        ? '#BBDEFB'
                        : '#FFFFFF'
                  }
                  stroke={selected ? '#1976D2' : vmtTheme.diagramLine}
                  strokeWidth={selected || linkHover ? 2 : 1}
                  cornerRadius={4}
                />
                <Text
                  listening={false}
                  x={-hw + pad}
                  y={-fontSize / 2}
                  width={m.size.x - pad * 2}
                  height={m.size.y}
                  align="center"
                  verticalAlign="middle"
                  text={label}
                  fontSize={fontSize}
                  fill={vmtTheme.textOnLight}
                  wrap="none"
                  ellipsis
                />
              </Group>
            );
          })}
        </Group>
      </Layer>
      {panOnly && width > 0 && height > 0 ? (
        <Layer>
          <Rect
            x={0}
            y={0}
            width={width}
            height={height}
            fill="rgba(0,0,0,0)"
            listening
          />
        </Layer>
      ) : null}
    </Stage>
  );
}
