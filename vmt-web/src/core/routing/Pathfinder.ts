import type { Point } from '../model/types';
import {
  appendUnique,
  isAxisSpike,
  lPathClear,
  manhattanCells,
  pointNearPolyline,
  segmentClear,
  type Path,
  type Rect,
  type WallChecker,
} from './geom';
import { RoutingMatrix } from './Matrix';

export class Pathfinder {
  private matrix!: RoutingMatrix;

  getPath(
    start: Point,
    finish: Point,
    bounds: Rect,
    gridSize: number,
    checker: WallChecker | null = null,
    blockedPaths: Path[] = [],
    pathCorridor = 6,
  ): Path {
    this.matrix = new RoutingMatrix(start, finish, bounds, gridSize);

    const combinedChecker: WallChecker = (point) =>
      this.pointBlocksPath(point, checker, blockedPaths, pathCorridor);

    this.matrix.fillWalls(combinedChecker);
    this.matrix.markBlockedPaths(blockedPaths, pathCorridor);

    const gridPath: Path = [];
    if (!this.searchPath(gridPath)) {
      return [];
    }

    let path = this.simplifyPath(gridPath);
    path = this.optimizeOrthogonalPath(path, combinedChecker);
    path = this.enforceTerminalDirections(path);
    return this.optimizeOrthogonalPath(path, combinedChecker);
  }

  private pointBlocksPath(
    point: Point,
    checker: WallChecker | null,
    blockedPaths: Path[],
    pathCorridor: number,
  ): boolean {
    if (checker?.(point)) return true;
    for (const blockedPath of blockedPaths) {
      if (pointNearPolyline(point, blockedPath, pathCorridor)) return true;
    }
    return false;
  }

  private enforceTerminalDirections(path: Path): Path {
    if (path.length < 2) return path;

    const stub = this.matrix.gridSize;
    const result: Path = [];
    appendUnique(result, path[0]!);

    if (path.length === 1 || path[1]!.x <= path[0]!.x) {
      appendUnique(result, { x: path[0]!.x + stub, y: path[0]!.y });
    }

    for (let i = 1; i < path.length; i++) {
      appendUnique(result, path[i]!);
    }

    if (result.length >= 2) {
      const prev = result[result.length - 2]!;
      const end = result[result.length - 1]!;
      if (end.x >= prev.x) {
        result.splice(result.length - 1, 0, { x: end.x - stub, y: end.y });
      }
    }

    return result;
  }

  private optimizeOrthogonalPath(path: Path, checker: WallChecker): Path {
    if (path.length < 3) return path;

    const sampleStep = Math.max(1, Math.floor(this.matrix.gridSize / 2));
    let changed = true;

    while (changed) {
      changed = false;

      for (let i = 1; i + 1 < path.length; ) {
        const a = path[i - 1]!;
        const b = path[i]!;
        const c = path[i + 1]!;

        if (isAxisSpike(a, b, c)) {
          path.splice(i, 1);
          changed = true;
          continue;
        }

        if (segmentClear(a, c, checker, sampleStep)) {
          path.splice(i, 1);
          changed = true;
          continue;
        }

        const { ok, corner } = lPathClear(a, c, checker, sampleStep);
        if (ok && corner && (b.x !== corner.x || b.y !== corner.y)) {
          if (
            (corner.x === a.x && corner.y === a.y) ||
            (corner.x === c.x && corner.y === c.y)
          ) {
            path.splice(i, 1);
          } else {
            path[i] = corner;
          }
          changed = true;
          continue;
        }

        i++;
      }
    }

    return path;
  }

  private searchPath(path: Path): boolean {
    const kTurnPenalty = 6;
    const cellCount = this.matrix.cellsCountX * this.matrix.cellsCountY;
    const stateCount = cellCount * 4;
    const gScore = new Array<number>(stateCount).fill(Number.MAX_SAFE_INTEGER);
    const parent = new Array<number>(stateCount).fill(-1);

    const startIndex = this.matrix.index(this.matrix.startCell);
    const finishIndex = this.matrix.index(this.matrix.finishCell);

    const stateId = (cellIndex: number, direction: number): number =>
      cellIndex * 4 + direction;

    const open: { fScore: number; state: number }[] = [];
    const pushOpen = (node: { fScore: number; state: number }) => {
      open.push(node);
    };
    const popOpen = (): { fScore: number; state: number } => {
      let minIdx = 0;
      for (let i = 1; i < open.length; i++) {
        if (open[i]!.fScore < open[minIdx]!.fScore) minIdx = i;
      }
      return open.splice(minIdx, 1)[0]!;
    };

    const directions: [number, number][] = [
      [1, 0],
      [-1, 0],
      [0, 1],
      [0, -1],
    ];
    const closed = new Array<boolean>(stateCount).fill(false);

    const startState = stateId(startIndex, 0);
    gScore[startState] = 0;
    parent[startState] = -1;
    pushOpen({
      fScore: manhattanCells(this.matrix.startCell, this.matrix.finishCell),
      state: startState,
    });

    while (open.length > 0) {
      const currentState = popOpen().state;
      if (closed[currentState]) continue;
      closed[currentState] = true;

      const currentIndex = Math.floor(currentState / 4);
      const incomingDirection = currentState % 4;

      if (currentIndex === finishIndex) {
        path.length = 0;
        let cursor = currentState;
        while (cursor >= 0) {
          const cellIndex = Math.floor(cursor / 4);
          const i = cellIndex % this.matrix.cellsCountX;
          const j = Math.floor(cellIndex / this.matrix.cellsCountX);
          path.push(this.matrix.cellToPoint([i, j]));
          if (cursor === startState) break;
          cursor = parent[cursor]!;
        }
        path.reverse();
        return true;
      }

      const currentI = currentIndex % this.matrix.cellsCountX;
      const currentJ = Math.floor(currentIndex / this.matrix.cellsCountX);

      for (let direction = 0; direction < 4; direction++) {
        const delta = directions[direction]!;
        const nextI = currentI + delta[0];
        const nextJ = currentJ + delta[1];
        if (
          nextI < 0 ||
          nextJ < 0 ||
          nextI >= this.matrix.cellsCountX ||
          nextJ >= this.matrix.cellsCountY
        ) {
          continue;
        }

        const next: [number, number] = [nextI, nextJ];
        if (this.matrix.at(next).wall) continue;

        const nextIndex = this.matrix.index(next);
        const nextState = stateId(nextIndex, direction);
        const turnCost =
          parent[currentState]! >= 0 &&
          currentIndex !== startIndex &&
          incomingDirection !== direction
            ? kTurnPenalty
            : 0;
        const tentative = gScore[currentState]! + 1 + turnCost;
        if (tentative >= gScore[nextState]!) continue;

        parent[nextState] = currentState;
        gScore[nextState] = tentative;
        const fScore = tentative + manhattanCells(next, this.matrix.finishCell);
        pushOpen({ fScore, state: nextState });
      }
    }

    path.length = 0;
    return false;
  }

  private simplifyPath(gridPath: Path): Path {
    if (gridPath.length <= 2) return gridPath;

    const cells: [number, number][] = gridPath.map((p) =>
      this.matrix.pointToCell(p),
    );

    const simplified: Path = [gridPath[0]!];
    let anchor = 0;
    for (let probe = anchor + 1; probe < cells.length; probe++) {
      if (!this.matrix.segmentIsClear(cells[anchor]!, cells[probe]!)) {
        simplified.push(gridPath[probe - 1]!);
        anchor = probe - 1;
      }
    }

    const last = gridPath[gridPath.length - 1]!;
    if (
      simplified[simplified.length - 1]!.x !== last.x ||
      simplified[simplified.length - 1]!.y !== last.y
    ) {
      simplified.push(last);
    }

    const orthogonal: Path = [simplified[0]!];
    for (let i = 1; i < simplified.length; i++) {
      const from = orthogonal[orthogonal.length - 1]!;
      const to = simplified[i]!;

      if (from.x !== to.x && from.y !== to.y) {
        let preferHorizontalFirst = true;
        if (orthogonal.length >= 2) {
          const prev = orthogonal[orthogonal.length - 2]!;
          preferHorizontalFirst = prev.y === from.y;
        } else if (to.x > from.x) {
          preferHorizontalFirst = true;
        } else if (to.x < from.x) {
          preferHorizontalFirst = false;
        }

        if (preferHorizontalFirst) {
          orthogonal.push({ x: to.x, y: from.y });
        } else {
          orthogonal.push({ x: from.x, y: to.y });
        }
      }

      const tail = orthogonal[orthogonal.length - 1]!;
      if (tail.x !== to.x || tail.y !== to.y) {
        orthogonal.push(to);
      }
    }

    return orthogonal;
  }
}
