import { describe, expect, it } from 'vitest';
import { appendGridToContext } from '../../src/core/routing/worldBounds';

describe('appendGridToContext', () => {
  it('starts each grid line with moveTo (orthogonal cells)', () => {
    const moves: { x: number; y: number }[] = [];
    const ctx = {
      beginPath: () => {},
      moveTo: (x: number, y: number) => moves.push({ x, y }),
      lineTo: () => {},
    };
    appendGridToContext(ctx, { x: 0, y: 0, width: 96, height: 96 }, 48);
    expect(moves).toHaveLength(6);
    expect(moves[0]).toEqual({ x: 0, y: 0 });
    expect(moves[3]).toEqual({ x: 0, y: 0 });
  });
});
