import { describe, expect, it } from 'vitest';
import {
  alignTapeViewToHead,
  defaultTapeViewStart,
  displayTapeSign,
  formatTapeWindow,
  parseTapeCellInput,
  TAPE_CELL_MIN_PX,
  TAPE_VIEW_DEFAULT_CELL_COUNT,
  TAPE_VIEW_RADIUS,
  tapeCellCountForWidth,
  tapeCellIndices,
  tapeViewWidth,
} from '../../src/core/tape/tapeViewport';

function mockTape(head: number, values: Record<number, string>) {
  return {
    getHeadIndex: () => head,
    getValueAt: (i: number) => values[i] ?? 'λ',
  };
}

describe('tapeViewport', () => {
  it('default view centers head', () => {
    expect(defaultTapeViewStart(0)).toBe(-TAPE_VIEW_RADIUS);
    expect(tapeViewWidth()).toBe(11);
  });

  it('formats window with head highlighted', () => {
    const tape = mockTape(2, { 0: 'a', 1: 'b', 2: 'c', 3: 'd' });
    const s = formatTapeWindow(tape, -3);
    expect(s).toContain('[c]');
    expect(s.length).toBeGreaterThan(5);
  });

  it('aligns view when head leaves window', () => {
    expect(alignTapeViewToHead(-5, 0)).toBe(-5);
    expect(alignTapeViewToHead(-5, 10)).toBe(10 - TAPE_VIEW_RADIUS);
    expect(alignTapeViewToHead(20, 5)).toBe(5 - TAPE_VIEW_RADIUS);
  });

  it('lists visible cell indices', () => {
    expect(tapeCellIndices(-3)).toEqual([-3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7]);
  });

  it('derives cell count from container width', () => {
    expect(tapeCellCountForWidth(0)).toBe(TAPE_VIEW_DEFAULT_CELL_COUNT);
    expect(tapeCellCountForWidth(400)).toBe(Math.floor(400 / TAPE_CELL_MIN_PX));
    expect(tapeCellCountForWidth(100)).toBe(5);
  });

  it('parses cell input against alphabet', () => {
    const alphabet = ' 01';
    expect(parseTapeCellInput('', alphabet)).toBe(' ');
    expect(parseTapeCellInput('λ', alphabet)).toBe(' ');
    expect(parseTapeCellInput('1', alphabet)).toBe('1');
    expect(parseTapeCellInput('x', alphabet)).toBeNull();
    expect(displayTapeSign(' ', ' ')).toBe('λ');
    expect(displayTapeSign('1', ' ')).toBe('1');
  });
});
