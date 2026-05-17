import { getLambda } from '../model/types';

const SHIFT_STEP = 10;

export class Tape {
  private blocks: string[][] = [];
  private zero = 0;
  private head = 0;
  private readonly lambda: string;

  constructor(alphabet: string) {
    this.lambda = getLambda(alphabet);
    this.ensurePosition(0);
  }

  getHeadIndex(): number {
    return this.head;
  }

  getValueAt(index: number): string {
    this.ensurePosition(index);
    const rel = index - this.zero;
    const blockIndex = Math.floor(rel / SHIFT_STEP);
    const inside = rel % SHIFT_STEP;
    return this.blocks[blockIndex]![inside]!;
  }

  getValueAtHead(): string {
    return this.getValueAt(this.head);
  }

  setValueAt(index: number, sign: string): void {
    this.ensurePosition(index);
    const rel = index - this.zero;
    const blockIndex = Math.floor(rel / SHIFT_STEP);
    const inside = rel % SHIFT_STEP;
    this.blocks[blockIndex]![inside] = sign;
  }

  setSignAtHead(sign: string): void {
    this.setValueAt(this.head, sign);
  }

  shiftLeft(): void {
    this.head -= 1;
    this.ensurePosition(this.head);
  }

  shiftRight(): void {
    this.head += 1;
    this.ensurePosition(this.head);
  }

  shiftLeftWord(): void {
    do {
      this.shiftLeft();
    } while (this.getValueAtHead() !== this.lambda);
  }

  shiftRightWord(): void {
    do {
      this.shiftRight();
    } while (this.getValueAtHead() !== this.lambda);
  }

  clear(): void {
    for (const block of this.blocks) {
      block.fill(this.lambda);
    }
  }

  private ensurePosition(index: number): void {
    if (index < this.zero) {
      const newZero = Math.floor((index - SHIFT_STEP) / SHIFT_STEP) * SHIFT_STEP;
      const add = (this.zero - newZero) / SHIFT_STEP;
      for (let i = 0; i < add; i++) {
        this.blocks.unshift(Array(SHIFT_STEP).fill(this.lambda));
      }
      this.zero = newZero;
    }
    const arrayIndex = Math.floor((index - this.zero) / SHIFT_STEP);
    while (arrayIndex >= this.blocks.length) {
      this.blocks.push(Array(SHIFT_STEP).fill(this.lambda));
    }
  }
}
