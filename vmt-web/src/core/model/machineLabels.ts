import type { Machine } from './types';
import { getLambda, isComplexMachine, isWriteMachine } from './types';

export function isLambdaSign(sign: string, alphabet: string): boolean {
  return sign === getLambda(alphabet);
}

/** Короткая подпись узла, вписываемая в квадрат (как на десктопе). */
export function machineDisplayLabel(machine: Machine, alphabet: string): string {
  switch (machine.type) {
    case 'start':
      return 'S';
    case 'finish':
      return 'F';
    case 'left':
      return 'L';
    case 'right':
      return 'R';
    case 'leftWord':
      return '⇐';
    case 'rightWord':
      return '⇒';
    case 'write':
      return isWriteMachine(machine) && isLambdaSign(machine.sign, alphabet)
        ? 'λ'
        : isWriteMachine(machine)
          ? machine.sign
          : 'λ';
    case 'copy':
      return 'C';
    case 'complex':
      return isComplexMachine(machine)
        ? truncateLabel(machine.name, 4)
        : 'M';
    default:
      return '?';
  }
}

export function machineLabelFontSize(machine: Machine): number {
  const h = machine.size.y;
  const w = machine.size.x;
  return Math.max(10, Math.min(16, Math.floor(Math.min(w, h) * 0.42)));
}

export function machineAcceptsOutgoing(machine: Machine): boolean {
  return machine.type !== 'finish';
}

export function machineAcceptsIncoming(machine: Machine): boolean {
  return machine.type !== 'start';
}

function truncateLabel(name: string, maxLen: number): string {
  const t = name.trim();
  if (t.length <= maxLen) return t || 'M';
  return `${t.slice(0, maxLen - 1)}…`;
}
