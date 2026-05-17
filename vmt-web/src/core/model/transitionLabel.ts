import type { Point, Transition, TransitionConditions } from './types';
import { getLambda } from './types';

/** Строка покрытых символов для проверок (как VMTTransitionConditions::GetAsString). */
export function transitionConditionSigns(
  alphabet: string,
  conditions: TransitionConditions,
): string {
  const lambda = getLambda(alphabet);
  const track = alphabet.split('').filter((c) => c !== lambda);
  const enabledTrack = conditions.enabledSigns.filter((s) => s !== lambda);

  if (track.length > 0 && enabledTrack.length >= track.length) {
    return alphabet;
  }
  return conditions.enabledSigns.join('');
}

export function formatTransitionConditions(
  alphabet: string,
  conditions: TransitionConditions,
): string {
  const lambda = getLambda(alphabet);
  const track = alphabet.split('').filter((c) => c !== lambda);
  if (track.length === 0) return '?';
  const enabled = conditions.enabledSigns.filter((s) => s !== lambda);
  if (enabled.length === 0) return '?';
  if (enabled.length >= track.length) return '*';
  return enabled.join('');
}

export function defaultTransitionConditions(alphabet: string): TransitionConditions {
  return {
    enabledSigns: alphabet.split(''),
  };
}

export function polylineLength(points: readonly Point[]): number {
  let len = 0;
  for (let i = 1; i < points.length; i++) {
    len += Math.hypot(
      points[i]!.x - points[i - 1]!.x,
      points[i]!.y - points[i - 1]!.y,
    );
  }
  return len;
}

export function pointOnPolyline(points: readonly Point[], t: number): Point {
  if (points.length === 0) return { x: 0, y: 0 };
  if (points.length === 1) return { ...points[0]! };
  const total = polylineLength(points);
  if (total <= 0) return { ...points[0]! };
  let target = total * Math.max(0, Math.min(1, t));
  for (let i = 1; i < points.length; i++) {
    const a = points[i - 1]!;
    const b = points[i]!;
    const seg = Math.hypot(b.x - a.x, b.y - a.y);
    if (target <= seg) {
      const u = seg === 0 ? 0 : target / seg;
      return { x: a.x + (b.x - a.x) * u, y: a.y + (b.y - a.y) * u };
    }
    target -= seg;
  }
  return { ...points[points.length - 1]! };
}

export function defaultConditionsAnchor(
  pathPoints: readonly Point[],
): Point {
  return pointOnPolyline(pathPoints, 0.5);
}

export function resolveLabelT(
  transition: Transition,
  pathPoints: readonly Point[],
): number {
  if (transition.conditionsLabelT !== undefined) {
    return Math.max(0, Math.min(1, transition.conditionsLabelT));
  }
  if (transition.conditionsAnchor && pathPoints.length >= 2) {
    return projectPointOnPolyline(transition.conditionsAnchor, pathPoints).t;
  }
  return 0.5;
}

export function resolveConditionsAnchor(
  transition: Transition,
  pathPoints: readonly Point[],
): Point {
  return pointOnPolyline(pathPoints, resolveLabelT(transition, pathPoints));
}

export function projectPointOnPolyline(
  point: Point,
  pathPoints: readonly Point[],
): { point: Point; t: number } {
  if (pathPoints.length < 2) {
    return { point: pathPoints[0] ?? point, t: 0 };
  }
  let bestDist = Number.MAX_VALUE;
  let bestPoint = pathPoints[0]!;
  let bestT = 0;
  const total = polylineLength(pathPoints);
  let walked = 0;

  for (let i = 1; i < pathPoints.length; i++) {
    const a = pathPoints[i - 1]!;
    const b = pathPoints[i]!;
    const dx = b.x - a.x;
    const dy = b.y - a.y;
    const segLen = Math.hypot(dx, dy);
    const u =
      segLen === 0
        ? 0
        : Math.max(
            0,
            Math.min(1, ((point.x - a.x) * dx + (point.y - a.y) * dy) / (segLen * segLen)),
          );
    const px = a.x + dx * u;
    const py = a.y + dy * u;
    const d = Math.hypot(point.x - px, point.y - py);
    if (d < bestDist) {
      bestDist = d;
      bestPoint = { x: px, y: py };
      bestT = total === 0 ? 0 : (walked + segLen * u) / total;
    }
    walked += segLen;
  }

  return { point: bestPoint, t: bestT };
}
