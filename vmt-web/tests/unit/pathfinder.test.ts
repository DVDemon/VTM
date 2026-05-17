import { describe, expect, it } from 'vitest';
import type { Point } from '@core/model/types';
import { Pathfinder } from '@core/routing/Pathfinder';
import type { Path, Rect } from '@core/routing/geom';

const GRID_STEP = 40;

function makeNode(centerX: number, centerY: number, half = 15): Rect {
  return {
    x: centerX - half,
    y: centerY - half,
    width: half * 2,
    height: half * 2,
  };
}

function outputPoint(rect: Rect): Point {
  return { x: rect.x + rect.width, y: rect.y + rect.height / 2 };
}

function inputPoint(rect: Rect): Point {
  return { x: rect.x, y: rect.y + rect.height / 2 };
}

function routeStart(output: Point): Point {
  return { x: output.x + GRID_STEP / 2, y: output.y };
}

function routeFinish(input: Point): Point {
  return { x: input.x - GRID_STEP / 2, y: input.y };
}

function totalBounds(nodes: Rect[], margin = 100): Rect {
  let bounds = { ...nodes[0]! };
  for (let i = 1; i < nodes.length; i++) {
    const n = nodes[i]!;
    bounds = {
      x: Math.min(bounds.x, n.x),
      y: Math.min(bounds.y, n.y),
      width: Math.max(bounds.x + bounds.width, n.x + n.width) - Math.min(bounds.x, n.x),
      height: Math.max(bounds.y + bounds.height, n.y + n.height) - Math.min(bounds.y, n.y),
    };
  }
  return {
    x: bounds.x - margin,
    y: bounds.y - margin,
    width: bounds.width + margin * 2,
    height: bounds.height + margin * 2,
  };
}

function nodeWallChecker(nodes: Rect[]): (p: Point) => boolean {
  return (point) =>
    nodes.some(
      (r) =>
        point.x >= r.x &&
        point.x <= r.x + r.width &&
        point.y >= r.y &&
        point.y <= r.y + r.height,
    );
}

function buildTransitionPath(
  nodes: Rect[],
  existing: Path[],
  from: number,
  to: number,
): Path {
  const pf = new Pathfinder();
  const start = routeStart(outputPoint(nodes[from]!));
  const finish = routeFinish(inputPoint(nodes[to]!));
  const middle = pf.getPath(
    start,
    finish,
    totalBounds(nodes),
    GRID_STEP,
    nodeWallChecker(nodes),
    existing,
    GRID_STEP / 2,
  );
  const full: Path = [outputPoint(nodes[from]!)];
  for (const p of middle) {
    const last = full[full.length - 1];
    if (!last || last.x !== p.x || last.y !== p.y) full.push(p);
  }
  full.push(inputPoint(nodes[to]!));
  return full;
}

describe('Pathfinder', () => {
  it('routes chain transitions around intermediate nodes', () => {
    const nodes = [0, 1, 2].map((i) => makeNode(100 + i * 120, 100));
    const paths: Path[] = [];
    for (let i = 0; i + 1 < nodes.length; i++) {
      paths.push(buildTransitionPath(nodes, paths, i, i + 1));
    }
    expect(paths.every((p) => p.length >= 2)).toBe(true);
    const middle = nodes[1]!;
    const blocker: Point = {
      x: middle.x + middle.width / 2,
      y: middle.y + middle.height / 2,
    };
    for (const path of paths) {
      const hitsCenter = path.some(
        (p) =>
          p.x >= middle.x &&
          p.x <= middle.x + middle.width &&
          p.y >= middle.y &&
          p.y <= middle.y + middle.height &&
          Math.abs(p.x - blocker.x) < 8 &&
          Math.abs(p.y - blocker.y) < 8,
      );
      expect(hitsCenter).toBe(false);
    }
  });

  it('dense blocker may return empty or detour around middle node', () => {
    const nodes = [
      makeNode(100, 100),
      makeNode(220, 100),
      makeNode(340, 100),
      makeNode(220, 220, 40),
    ];
    const path = buildTransitionPath(nodes, [], 0, 2);
    const middle = nodes[1]!;
    const throughMiddle =
      path.length > 0 &&
      path.some(
        (p) =>
          p.x >= middle.x &&
          p.x <= middle.x + middle.width &&
          p.y >= middle.y &&
          p.y <= middle.y + middle.height,
      );
    expect(path.length === 0 || !throughMiddle).toBe(true);
  });
});
