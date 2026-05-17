import type { Point } from '../model/types';
import type { Rect, WallChecker } from './geom';
import type { Path } from './geom';

const K_MARGIN = 100;
const K_MAX_CELLS_PER_AXIS = 128;

interface Cell {
  wall: boolean;
}

export class RoutingMatrix {
  readonly start: Point;
  readonly finish: Point;
  readonly gridSize: number;
  bounds: Rect;
  cellsCountX = 0;
  cellsCountY = 0;
  startCell: [number, number] = [0, 0];
  finishCell: [number, number] = [0, 0];
  private cells: Cell[] = [];

  constructor(start: Point, finish: Point, bounds: Rect, gridSize: number) {
    this.start = start;
    this.finish = finish;
    this.gridSize = gridSize;

    this.bounds = {
      x: bounds.x - K_MARGIN,
      y: bounds.y - K_MARGIN,
      width: bounds.width + K_MARGIN * 2,
      height: bounds.height + K_MARGIN * 2,
    };
    const startFinish = rectNormalized(start, finish);
    this.bounds = rectUnion(this.bounds, startFinish);

    const gs = gridSize > 0 ? gridSize : 1;
    const bottom =
      Math.floor(this.bounds.y + this.bounds.height / gs) * gs + Math.floor(gs / 2);
    const top = Math.floor(this.bounds.y / gs) * gs + Math.floor(gs / 2);
    this.bounds = {
      ...this.bounds,
      y: top,
      height: bottom - top,
    };
    this.bounds = {
      ...this.bounds,
      width: Math.floor(this.bounds.width / gs) * gs,
      height: Math.floor(this.bounds.height / gs) * gs,
    };

    let effectiveGrid = gs;
    const computeAxisCells = (dimension: number): number => {
      let cells = Math.ceil(dimension / effectiveGrid);
      if (cells < 2) cells = 2;
      return cells;
    };

    this.cellsCountX = computeAxisCells(this.bounds.width);
    this.cellsCountY = computeAxisCells(this.bounds.height);
    while (
      this.cellsCountX > K_MAX_CELLS_PER_AXIS ||
      this.cellsCountY > K_MAX_CELLS_PER_AXIS
    ) {
      effectiveGrid *= 2;
      this.cellsCountX = computeAxisCells(this.bounds.width);
      this.cellsCountY = computeAxisCells(this.bounds.height);
    }

    this.cells = Array.from({ length: this.cellsCountX * this.cellsCountY }, () => ({
      wall: false,
    }));
    this.startCell = this.pointToCell(start);
    this.finishCell = this.pointToCell(finish);
    this.at(this.startCell).wall = false;
    this.at(this.finishCell).wall = false;
  }

  index(cell: [number, number]): number {
    return cell[0] + cell[1] * this.cellsCountX;
  }

  at(cell: [number, number]): Cell {
    return this.cells[this.index(cell)]!;
  }

  cellToPoint(cell: [number, number]): Point {
    return {
      x:
        this.bounds.x +
        Math.floor((cell[0] * this.bounds.width) / this.cellsCountX),
      y:
        this.bounds.y +
        Math.floor((cell[1] * this.bounds.height) / this.cellsCountY),
    };
  }

  pointToCell(point: Point): [number, number] {
    if (this.cellsCountX === 0 || this.cellsCountY === 0) return [0, 0];

    const cellWidth = Math.max(
      1,
      Math.floor(this.bounds.width / this.cellsCountX),
    );
    const cellHeight = Math.max(
      1,
      Math.floor(this.bounds.height / this.cellsCountY),
    );

    let bestI = 0;
    let bestJ = 0;
    let bestDistance = Number.MAX_SAFE_INTEGER;

    const guessI = Math.min(
      this.cellsCountX - 1,
      Math.max(0, Math.floor((point.x - this.bounds.x) / cellWidth)),
    );
    const guessJ = Math.min(
      this.cellsCountY - 1,
      Math.max(0, Math.floor((point.y - this.bounds.y) / cellHeight)),
    );

    for (let di = 0; di <= 1; di++) {
      for (let dj = 0; dj <= 1; dj++) {
        const i = guessI + di - 1;
        const j = guessJ + dj - 1;
        if (
          i < 0 ||
          j < 0 ||
          i >= this.cellsCountX ||
          j >= this.cellsCountY
        ) {
          continue;
        }
        const center = this.cellToPoint([i, j]);
        const distance =
          Math.abs(center.x - point.x) + Math.abs(center.y - point.y);
        if (distance < bestDistance) {
          bestDistance = distance;
          bestI = i;
          bestJ = j;
        }
      }
    }

    return [bestI, bestJ];
  }

  fillWalls(checker: WallChecker): void {
    for (let i = 0; i < this.cellsCountX; i++) {
      for (let j = 0; j < this.cellsCountY; j++) {
        const cell: [number, number] = [i, j];
        if (
          cell[0] === this.startCell[0] &&
          cell[1] === this.startCell[1]
        ) {
          this.at(cell).wall = false;
          continue;
        }
        if (
          cell[0] === this.finishCell[0] &&
          cell[1] === this.finishCell[1]
        ) {
          this.at(cell).wall = false;
          continue;
        }
        this.at(cell).wall = checker(this.cellToPoint(cell));
      }
    }
  }

  markBlockedPaths(blockedPaths: Path[], pathCorridor: number): void {
    for (const blockedPath of blockedPaths) {
      if (blockedPath.length < 2) continue;

      for (let segmentIndex = 1; segmentIndex < blockedPath.length; segmentIndex++) {
        const from = blockedPath[segmentIndex - 1]!;
        const to = blockedPath[segmentIndex]!;
        const steps = Math.max(
          Math.abs(to.x - from.x),
          Math.abs(to.y - from.y),
        );
        const stride = Math.max(
          1,
          Math.floor(steps / Math.max(this.cellsCountX, this.cellsCountY)) + 1,
        );

        for (let step = 0; step <= steps; step += stride) {
          const t = steps === 0 ? 0 : step / steps;
          const sample: Point = {
            x: Math.round(from.x + (to.x - from.x) * t),
            y: Math.round(from.y + (to.y - from.y) * t),
          };
          const half = Math.floor(pathCorridor / 2) + 1;
          for (let dx = -pathCorridor; dx <= pathCorridor; dx += half) {
            for (let dy = -pathCorridor; dy <= pathCorridor; dy += half) {
              const probe: Point = { x: sample.x + dx, y: sample.y + dy };
              const cell = this.pointToCell(probe);
              if (
                (cell[0] === this.startCell[0] &&
                  cell[1] === this.startCell[1]) ||
                (cell[0] === this.finishCell[0] &&
                  cell[1] === this.finishCell[1])
              ) {
                continue;
              }
              this.at(cell).wall = true;
            }
          }
        }
      }
    }
  }

  segmentIsClear(from: [number, number], to: [number, number]): boolean {
    const x0 = from[0];
    const y0 = from[1];
    const x1 = to[0];
    const y1 = to[1];

    const dx = Math.abs(x1 - x0);
    const dy = Math.abs(y1 - y0);
    const sx = x0 < x1 ? 1 : -1;
    const sy = y0 < y1 ? 1 : -1;
    let err = dx - dy;

    let x = x0;
    let y = y0;

    while (true) {
      if (
        x < 0 ||
        y < 0 ||
        x >= this.cellsCountX ||
        y >= this.cellsCountY
      ) {
        return false;
      }

      const cell: [number, number] = [x, y];
      if (
        !(cell[0] === from[0] && cell[1] === from[1]) &&
        !(cell[0] === to[0] && cell[1] === to[1]) &&
        this.at(cell).wall
      ) {
        return false;
      }

      if (x === x1 && y === y1) break;

      const err2 = err * 2;
      if (err2 > -dy) {
        err -= dy;
        x += sx;
      }
      if (err2 < dx) {
        err += dx;
        y += sy;
      }
    }

    return true;
  }
}

function rectUnion(a: Rect, b: Rect): Rect {
  const x1 = Math.min(a.x, b.x);
  const y1 = Math.min(a.y, b.y);
  const x2 = Math.max(a.x + a.width, b.x + b.width);
  const y2 = Math.max(a.y + a.height, b.y + b.height);
  return { x: x1, y: y1, width: x2 - x1, height: y2 - y1 };
}

function rectNormalized(a: Point, b: Point): Rect {
  const x = Math.min(a.x, b.x);
  const y = Math.min(a.y, b.y);
  return {
    x,
    y,
    width: Math.abs(a.x - b.x) || 1,
    height: Math.abs(a.y - b.y) || 1,
  };
}
