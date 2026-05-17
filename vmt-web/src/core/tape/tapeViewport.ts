import { isAlphabetSign } from '../model/alphabet';
import { getLambda } from '../model/types';

/** Default visible cells before layout measure (≈ desktop minimum). */
export const TAPE_VIEW_DEFAULT_CELL_COUNT = 11;

export const TAPE_VIEW_MIN_CELLS = 5;

/** Minimum cell width (px) used to fit cells across the container. */
export const TAPE_CELL_MIN_PX = 36;

/** @deprecated Prefer `cellCount`; half-window for 11 cells. */
export const TAPE_VIEW_RADIUS = Math.floor((TAPE_VIEW_DEFAULT_CELL_COUNT - 1) / 2);

export function tapeCellCountForWidth(containerWidth: number): number {
  if (containerWidth <= 0) return TAPE_VIEW_DEFAULT_CELL_COUNT;
  return Math.max(
    TAPE_VIEW_MIN_CELLS,
    Math.floor(containerWidth / TAPE_CELL_MIN_PX),
  );
}

export function displayTapeSign(sign: string, lambda: string): string {
  return sign === lambda ? 'λ' : sign;
}

/** Parse one cell edit; `null` = reject (keep previous value). */
export function parseTapeCellInput(
  raw: string,
  alphabet: string,
  lambda?: string,
): string | null {
  const lam = lambda ?? getLambda(alphabet);
  const text = raw.trim();
  if (text === '') return lam;
  const ch = text.length > 1 ? text.slice(-1) : text[0]!;
  if (ch === 'λ') return lam;
  if (isAlphabetSign(alphabet, ch)) return ch;
  return null;
}

export interface TapeReadView {
  getHeadIndex(): number;
  getValueAt(index: number): string;
}

export function tapeViewWidth(cellCount = TAPE_VIEW_DEFAULT_CELL_COUNT): number {
  return cellCount;
}

function cellsBeforeHead(cellCount: number): number {
  return Math.floor((cellCount - 1) / 2);
}

export function defaultTapeViewStart(
  headIndex: number,
  cellCount = TAPE_VIEW_DEFAULT_CELL_COUNT,
): number {
  return headIndex - cellsBeforeHead(cellCount);
}

/** Keep head inside the visible window (desktop `FormLineWidget::Repaint`). */
export function alignTapeViewToHead(
  viewStart: number,
  headIndex: number,
  cellCount = TAPE_VIEW_DEFAULT_CELL_COUNT,
): number {
  const right = viewStart + cellCount - 1;
  const before = cellsBeforeHead(cellCount);
  if (headIndex < viewStart) {
    return headIndex - before;
  }
  if (headIndex > right) {
    return headIndex - before;
  }
  return viewStart;
}

export function tapeCellIndices(
  viewStart: number,
  cellCount = TAPE_VIEW_DEFAULT_CELL_COUNT,
): number[] {
  return Array.from({ length: cellCount }, (_, i) => viewStart + i);
}

export function formatTapeWindow(
  tape: TapeReadView,
  viewStart: number,
  cellCount = TAPE_VIEW_DEFAULT_CELL_COUNT,
): string {
  const head = tape.getHeadIndex();
  const chars: string[] = [];
  for (let i = viewStart; i < viewStart + cellCount; i++) {
    const cell = tape.getValueAt(i);
    chars.push(i === head ? `[${cell}]` : cell);
  }
  return chars.join('');
}
