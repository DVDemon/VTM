import type { Point } from '../model/types';
import type { Path } from './geom';

export interface CubicSegment {
  readonly p0: Point;
  readonly c1: Point;
  readonly c2: Point;
  readonly p3: Point;
}

function extrapolateStart(points: Point[]): Point {
  return {
    x: points[0]!.x + (points[0]!.x - points[1]!.x),
    y: points[0]!.y + (points[0]!.y - points[1]!.y),
  };
}

function extrapolateEnd(points: Point[]): Point {
  const last = points[points.length - 1]!;
  const prev = points[points.length - 2]!;
  return {
    x: last.x + (last.x - prev.x),
    y: last.y + (last.y - prev.y),
  };
}

function pointAt(points: Point[], index: number): Point {
  if (index < 0) return extrapolateStart(points);
  if (index >= points.length) return extrapolateEnd(points);
  return points[index]!;
}

export function snapPortHeights(points: Point[]): Point[] {
  if (points.length < 2) return points;

  const result = points.map((p) => ({ ...p }));
  const startY = result[0]!.y;
  const endY = result[result.length - 1]!.y;

  result[0]!.y = startY;
  result[result.length - 1]!.y = endY;
  if (result.length >= 2) result[1]!.y = startY;
  if (result.length >= 3) result[result.length - 2]!.y = endY;

  return result;
}

export function buildConnectorPath(points: Point[]): CubicSegment[] {
  if (points.length < 2) return [];

  if (points.length === 2) {
    return [
      {
        p0: points[0]!,
        c1: points[0]!,
        c2: points[1]!,
        p3: points[1]!,
      },
    ];
  }

  const segments: CubicSegment[] = [];
  for (let i = 0; i + 1 < points.length; i++) {
    const p0 = pointAt(points, i - 1);
    const p1 = points[i]!;
    const p2 = points[i + 1]!;
    const p3 = pointAt(points, i + 2);

    let c1: Point = {
      x: p1.x + (p2.x - p0.x) / 6,
      y: p1.y + (p2.y - p0.y) / 6,
    };
    let c2: Point = {
      x: p2.x - (p3.x - p1.x) / 6,
      y: p2.y - (p3.y - p1.y) / 6,
    };

    if (i === 0) c1 = { ...c1, y: p1.y };
    if (i + 1 === points.length - 1) c2 = { ...c2, y: p2.y };

    segments.push({ p0: p1, c1, c2, p3: p2 });
  }

  return segments;
}

function cubicAt(seg: CubicSegment, t: number): Point {
  const u = 1 - t;
  const uu = u * u;
  const tt = t * t;
  const uuu = uu * u;
  const ttt = tt * t;

  return {
    x:
      uuu * seg.p0.x +
      3 * uu * t * seg.c1.x +
      3 * u * tt * seg.c2.x +
      ttt * seg.p3.x,
    y:
      uuu * seg.p0.y +
      3 * uu * t * seg.c1.y +
      3 * u * tt * seg.c2.y +
      ttt * seg.p3.y,
  };
}

function segmentLength(seg: CubicSegment, samples = 16): number {
  let length = 0;
  let prev = cubicAt(seg, 0);
  for (let i = 1; i <= samples; i++) {
    const next = cubicAt(seg, i / samples);
    length += Math.hypot(next.x - prev.x, next.y - prev.y);
    prev = next;
  }
  return length;
}

export function flattenPath(
  segments: CubicSegment[],
  maxSegmentLength = 8,
): Path {
  const samples: Path = [];
  if (segments.length === 0) return samples;

  for (const seg of segments) {
    const length = segmentLength(seg);
    const steps = Math.max(2, Math.ceil(length / Math.max(maxSegmentLength, 1)));
    for (let i = 0; i <= steps; i++) {
      const point = cubicAt(seg, i / steps);
      const last = samples[samples.length - 1];
      if (!last || last.x !== point.x || last.y !== point.y) {
        samples.push(point);
      }
    }
  }

  return samples;
}

function distanceToSegment(point: Point, a: Point, b: Point): number {
  const dx = b.x - a.x;
  const dy = b.y - a.y;
  if (dx === 0 && dy === 0) {
    return Math.hypot(point.x - a.x, point.y - a.y);
  }
  const t = Math.max(
    0,
    Math.min(1, ((point.x - a.x) * dx + (point.y - a.y) * dy) / (dx * dx + dy * dy)),
  );
  const px = a.x + t * dx;
  const py = a.y + t * dy;
  return Math.hypot(point.x - px, point.y - py);
}

export function isPointNearStroke(
  segments: CubicSegment[],
  point: Point,
  tolerance: number,
): boolean {
  const polyline = flattenPath(segments, Math.max(2, tolerance));
  for (let i = 1; i < polyline.length; i++) {
    if (
      distanceToSegment(point, polyline[i - 1]!, polyline[i]!) <= tolerance
    ) {
      return true;
    }
  }
  return false;
}

/** Konva sceneFunc: draw smooth connector through routing points. */
export function drawConnectorPath(
  ctx: CanvasRenderingContext2D,
  points: Point[],
): void {
  const segments = buildConnectorPath(snapPortHeights(points));
  if (segments.length === 0) return;

  const first = segments[0]!.p0;
  ctx.moveTo(first.x, first.y);
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
}
