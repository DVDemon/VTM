import type { DebuggerState } from './Debugger';
import { findBody, findMachine } from '../model/factory';
import { isComplexMachine } from '../model/types';

export interface CallStackEntry {
  readonly depth: number;
  readonly bodyName: string;
  readonly machineLabel: string;
}

export function formatDebuggerCallStack(state: DebuggerState): CallStackEntry[] {
  const entries: CallStackEntry[] = [];
  state.callStack.forEach((frame, index) => {
    const body = findBody(state.project, frame.bodyId);
    const machine = body ? findMachine(body, frame.machineId) : undefined;
    entries.push({
      depth: index,
      bodyName: body?.name ?? frame.bodyId.slice(0, 8),
      machineLabel: machineLabel(machine),
    });
  });
  const currentBody = findBody(state.project, state.bodyId);
  const currentMachine = currentBody
    ? findMachine(currentBody, state.machineId)
    : undefined;
  entries.push({
    depth: state.callStack.length,
    bodyName: currentBody?.name ?? state.bodyId.slice(0, 8),
    machineLabel: machineLabel(currentMachine),
  });
  return entries;
}

function machineLabel(machine: ReturnType<typeof findMachine>): string {
  if (!machine) return '?';
  if (isComplexMachine(machine)) return machine.name;
  return machine.type;
}
