import { describe, expect, it } from 'vitest';
import { Tape } from '@core/tape/Tape';

describe('Tape', () => {
  it('expands left when shifting beyond zero', () => {
    const tape = new Tape(' ');
    tape.setValueAt(0, 'A');
    tape.shiftLeft();
    expect(tape.getValueAt(-1)).toBe(' ');
    expect(tape.getValueAt(0)).toBe('A');
  });

  it('expands right when shifting far right', () => {
    const tape = new Tape(' ');
    for (let i = 0; i < 25; i++) tape.shiftRight();
    tape.setSignAtHead('B');
    expect(tape.getValueAtHead()).toBe('B');
  });

  it('shiftRightWord stops at lambda', () => {
    const tape = new Tape(' 01');
    tape.setValueAt(0, '1');
    tape.setValueAt(1, ' ');
    tape.shiftRight();
    tape.shiftRightWord();
    expect(tape.getValueAtHead()).toBe(' ');
  });
});
