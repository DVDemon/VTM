import type { Machine, Point } from '../model/types';
import type { Path, Rect } from './geom';
import { machineRect } from './transitionRoute';

const DEFAULT_PADDING = 96;

function includePoint(bounds: Rect, p: Point): Rect {
  const x2 = p.x;
  const y2 = p.y;
  const x1 = Math.min(bounds.x, x2);
  const y1 = Math.min(bounds.y, y2);
  const xMax = Math.max(bounds.x + bounds.width, x2);
  const yMax = Math.max(bounds.y + bounds.height, y2);
  return { x: x1, y: y1, width: xMax - x1, height: yMax - y1 };
}

function includeRect(bounds: Rect, r: Rect): Rect {
  return includePoint(
    includePoint(bounds, { x: r.x, y: r.y }),
    { x: r.x + r.width, y: r.y + r.height },
  );
}

/** Мировая область диаграммы: контент + видимый viewport при pan. */
export function computeWorldSurface(
  machines: readonly Machine[],
  paths: readonly Path[],
  viewOffset: Point,
  viewportWidth: number,
  viewportHeight: number,
  padding = DEFAULT_PADDING,
): Rect {
  const visLeft = -viewOffset.x;
  const visTop = -viewOffset.y;
  let bounds: Rect = {
    x: visLeft,
    y: visTop,
    width: viewportWidth,
    height: viewportHeight,
  };

  for (const m of machines) {
    bounds = includeRect(bounds, machineRect(m));
  }
  for (const path of paths) {
    for (const p of path) {
      bounds = includePoint(bounds, p);
    }
  }

  return {
    x: bounds.x - padding,
    y: bounds.y - padding,
    width: bounds.width + padding * 2,
    height: bounds.height + padding * 2,
  };
}

export type GridPathContext = {
  beginPath(): void;
  moveTo(x: number, y: number): void;
  lineTo(x: number, y: number): void;
};

/** Orthogonal grid: each line starts with moveTo (no diagonal connectors). */
export function appendGridToContext(
  ctx: GridPathContext,
  bounds: Rect,
  cell: number,
): void {
  const x0 = Math.floor(bounds.x / cell) * cell;
  const y0 = Math.floor(bounds.y / cell) * cell;
  const x1 = bounds.x + bounds.width;
  const y1 = bounds.y + bounds.height;

  ctx.beginPath();
  for (let x = x0; x <= x1; x += cell) {
    ctx.moveTo(x, bounds.y);
    ctx.lineTo(x, y1);
  }
  for (let y = y0; y <= y1; y += cell) {
    ctx.moveTo(bounds.x, y);
    ctx.lineTo(x1, y);
  }
}
