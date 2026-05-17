import type { Machine, Point, Transition } from '../model/types';
import { Pathfinder } from './Pathfinder';
import {
  rectMargins,
  rectUnion,
  type Path,
  type Rect,
  type WallChecker,
} from './geom';
import { snapPortHeights } from './diagramBezier';

export const DEFAULT_GRID_STEP = 40;

export function machineRect(machine: Machine): Rect {
  return {
    x: machine.center.x - machine.size.x / 2,
    y: machine.center.y - machine.size.y / 2,
    width: machine.size.x,
    height: machine.size.y,
  };
}

export function outputPoint(rect: Rect): Point {
  return { x: rect.x + rect.width, y: rect.y + rect.height / 2 };
}

export function inputPoint(rect: Rect): Point {
  return { x: rect.x, y: rect.y + rect.height / 2 };
}

function routeStartPoint(output: Point, gridStep: number): Point {
  return { x: output.x + Math.floor(gridStep / 2), y: output.y };
}

function routeFinishPoint(input: Point, gridStep: number): Point {
  return { x: input.x - Math.floor(gridStep / 2), y: input.y };
}

export function makeNodeWallChecker(
  machines: readonly Machine[],
  clearance = 8,
  excludeMachineIds: readonly string[] = [],
): WallChecker {
  const skip = new Set(excludeMachineIds);
  const rects = machines
    .filter((m) => !skip.has(m.id))
    .map((m) => {
      const r = machineRect(m);
      return {
        x: r.x - clearance,
        y: r.y - clearance,
        width: r.width + clearance * 2,
        height: r.height + clearance * 2,
      };
    });

  return (point) =>
    rects.some(
      (r) =>
        point.x >= r.x &&
        point.x <= r.x + r.width &&
        point.y >= r.y &&
        point.y <= r.y + r.height,
    );
}

/** Пересчёт всех связей с учётом порядка и уже проложенных путей. */
export function routeAllTransitions(
  machines: readonly Machine[],
  transitions: readonly Transition[],
  gridStep = DEFAULT_GRID_STEP,
): Map<string, Path> {
  const blocked: Path[] = [];
  const result = new Map<string, Path>();
  for (const t of transitions) {
    const path = routeTransitionPolyline(machines, t, blocked, gridStep);
    result.set(t.id, path);
    if (path.length >= 2) {
      blocked.push(path);
    }
  }
  return result;
}

export function routeTransitionPolyline(
  machines: readonly Machine[],
  transition: Transition,
  blockedPaths: Path[] = [],
  gridStep = DEFAULT_GRID_STEP,
): Path {
  const byId = new Map(machines.map((m) => [m.id, m]));
  const startMachine = byId.get(transition.startMachineId);
  const finishMachine = byId.get(transition.finishMachineId);
  if (!startMachine || !finishMachine) return [];

  const startOut = outputPoint(machineRect(startMachine));
  const finishIn = inputPoint(machineRect(finishMachine));
  const routeStart = routeStartPoint(startOut, gridStep);
  const routeFinish = routeFinishPoint(finishIn, gridStep);

  let bounds = rectUnion(machineRect(startMachine), machineRect(finishMachine));
  for (const path of blockedPaths) {
    for (const p of path) {
      bounds = rectUnion(bounds, {
        x: p.x,
        y: p.y,
        width: 1,
        height: 1,
      });
    }
  }
  bounds = rectMargins(bounds, 200);

  const exclude = [transition.startMachineId, transition.finishMachineId];
  const pathfinder = new Pathfinder();
  const corridor = Math.floor(gridStep / 2);
  const middle = pathfinder.getPath(
    routeStart,
    routeFinish,
    bounds,
    gridStep,
    makeNodeWallChecker(machines, corridor, exclude),
    blockedPaths,
    corridor,
  );

  if (middle.length === 0) {
    const corner: Point = { x: routeFinish.x, y: routeStart.y };
    return snapPortHeights([startOut, corner, finishIn]);
  }

  const full: Path = [startOut];
  for (const p of middle) {
    const last = full[full.length - 1];
    if (!last || last.x !== p.x || last.y !== p.y) full.push(p);
  }
  const last = full[full.length - 1];
  if (!last || last.x !== finishIn.x || last.y !== finishIn.y) {
    full.push(finishIn);
  }

  return snapPortHeights(full);
}
