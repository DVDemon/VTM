import type { Point } from '../model/types';

export interface Rect {
  readonly x: number;
  readonly y: number;
  readonly width: number;
  readonly height: number;
}

export type Path = Point[];

export type WallChecker = (point: Point) => boolean;

export function rectNormalized(a: Point, b: Point): Rect {
  const x = Math.min(a.x, b.x);
  const y = Math.min(a.y, b.y);
  return {
    x,
    y,
    width: Math.abs(a.x - b.x),
    height: Math.abs(a.y - b.y),
  };
}

export function rectUnion(a: Rect, b: Rect): Rect {
  const x1 = Math.min(a.x, b.x);
  const y1 = Math.min(a.y, b.y);
  const x2 = Math.max(a.x + a.width, b.x + b.width);
  const y2 = Math.max(a.y + a.height, b.y + b.height);
  return { x: x1, y: y1, width: x2 - x1, height: y2 - y1 };
}

export function rectMargins(rect: Rect, margin: number): Rect {
  return {
    x: rect.x - margin,
    y: rect.y - margin,
    width: rect.width + margin * 2,
    height: rect.height + margin * 2,
  };
}

export function pointNearSegment(
  point: Point,
  a: Point,
  b: Point,
  tolerance: number,
): boolean {
  if (a.x === b.x) {
    if (Math.abs(point.x - a.x) > tolerance) return false;
    const minY = Math.min(a.y, b.y);
    const maxY = Math.max(a.y, b.y);
    return point.y >= minY - tolerance && point.y <= maxY + tolerance;
  }
  if (a.y === b.y) {
    if (Math.abs(point.y - a.y) > tolerance) return false;
    const minX = Math.min(a.x, b.x);
    const maxX = Math.max(a.x, b.x);
    return point.x >= minX - tolerance && point.x <= maxX + tolerance;
  }
  return false;
}

export function pointNearPolyline(
  point: Point,
  polyline: Path,
  tolerance: number,
): boolean {
  if (polyline.length < 2) return false;
  for (let i = 1; i < polyline.length; i++) {
    if (pointNearSegment(point, polyline[i - 1]!, polyline[i]!, tolerance)) {
      return true;
    }
  }
  return false;
}

export function valueBetween(value: number, a: number, b: number): boolean {
  const low = Math.min(a, b);
  const high = Math.max(a, b);
  return value >= low && value <= high;
}

export function segmentClear(
  from: Point,
  to: Point,
  checker: WallChecker | null,
  sampleStep: number,
): boolean {
  if (!checker) return true;
  if (from.x === to.x && from.y === to.y) return !checker(from);

  const step = Math.max(1, sampleStep);
  if (from.x === to.x) {
    const stride = to.y >= from.y ? step : -step;
    for (let y = from.y; stride > 0 ? y <= to.y : y >= to.y; y += stride) {
      if (checker({ x: from.x, y })) return false;
    }
    return true;
  }
  if (from.y === to.y) {
    const stride = to.x >= from.x ? step : -step;
    for (let x = from.x; stride > 0 ? x <= to.x : x >= to.x; x += stride) {
      if (checker({ x, y: from.y })) return false;
    }
    return true;
  }
  return false;
}

export function lPathClear(
  from: Point,
  to: Point,
  checker: WallChecker | null,
  sampleStep: number,
): { ok: boolean; corner?: Point } {
  const cornerH: Point = { x: to.x, y: from.y };
  const cornerV: Point = { x: from.x, y: to.y };
  if (
    segmentClear(from, cornerH, checker, sampleStep) &&
    segmentClear(cornerH, to, checker, sampleStep)
  ) {
    return { ok: true, corner: cornerH };
  }
  if (
    segmentClear(from, cornerV, checker, sampleStep) &&
    segmentClear(cornerV, to, checker, sampleStep)
  ) {
    return { ok: true, corner: cornerV };
  }
  return { ok: false };
}

export function appendUnique(path: Path, point: Point): void {
  if (path.length === 0 || path[path.length - 1]!.x !== point.x || path[path.length - 1]!.y !== point.y) {
    path.push(point);
  }
}

export function isAxisSpike(a: Point, b: Point, c: Point): boolean {
  if (a.x === b.x && b.x === c.x) {
    return (
      valueBetween(b.y, a.y, c.y) && b.y !== a.y && b.y !== c.y
    );
  }
  if (a.y === b.y && b.y === c.y) {
    return (
      valueBetween(b.x, a.x, c.x) && b.x !== a.x && b.x !== c.x
    );
  }
  return false;
}

export function manhattanCells(
  a: [number, number],
  b: [number, number],
): number {
  return Math.abs(a[0] - b[0]) + Math.abs(a[1] - b[1]);
}
