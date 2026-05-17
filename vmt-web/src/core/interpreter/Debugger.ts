import type { Machine, MachineBody, Project } from '../model/types';
import { isComplexMachine, isWriteMachine } from '../model/types';
import { findBody, findMachine, getRootBody } from '../model/factory';
import { Tape } from '../tape/Tape';

export interface StackFrame {
  readonly bodyId: string;
  readonly machineId: string;
  readonly repeatIndex: number;
}

export interface DebuggerState {
  readonly project: Project;
  readonly bodyId: string;
  readonly machineId: string;
  readonly callStack: readonly StackFrame[];
  readonly tape: Tape;
  readonly finished: boolean;
}

export function createDebugger(project: Project): DebuggerState {
  const root = getRootBody(project);
  const start = root.machines.find((m) => m.type === 'start');
  if (!start) {
    throw new Error('No start machine in root body');
  }
  return {
    project,
    bodyId: root.id,
    machineId: start.id,
    callStack: [],
    tape: new Tape(project.alphabet),
    finished: false,
  };
}

function findNextMachine(
  body: MachineBody,
  machine: Machine,
  tape: Tape,
): Machine | null {
  const sign = tape.getValueAtHead();
  for (const t of body.transitions) {
    if (t.startMachineId !== machine.id) continue;
    if (t.conditions.enabledSigns.includes(sign)) {
      return findMachine(body, t.finishMachineId) ?? null;
    }
  }
  return machine;
}

function pushStack(
  state: DebuggerState,
  frame: StackFrame,
  times: number,
): DebuggerState {
  const stack = [...state.callStack];
  for (let i = 0; i < times; i++) stack.push(frame);
  return { ...state, callStack: stack };
}

export function stepDebugger(state: DebuggerState): DebuggerState {
  if (state.finished) return state;

  const body = findBody(state.project, state.bodyId);
  if (!body) return state;
  const machine = findMachine(body, state.machineId);
  if (!machine) return state;

  let next: DebuggerState = state;

  switch (machine.type) {
    case 'start': {
      const target = findNextMachine(body, machine, state.tape);
      if (target) {
        next = { ...state, machineId: target.id };
      }
      break;
    }
    case 'finish': {
      if (state.callStack.length === 0) {
        next = { ...state, finished: true };
      } else {
        const frame = state.callStack[state.callStack.length - 1]!;
        const stack = state.callStack.slice(0, -1);
        const parentBody = findBody(state.project, frame.bodyId);
        if (!parentBody) break;
        const parentMachine = findMachine(parentBody, frame.machineId);
        if (!parentMachine) break;
        const after = findNextMachine(parentBody, parentMachine, state.tape);
        next = {
          ...state,
          callStack: stack,
          bodyId: frame.bodyId,
          machineId: after?.id ?? frame.machineId,
        };
      }
      break;
    }
    case 'complex': {
      if (!isComplexMachine(machine)) break;
      const inner = state.project.bodies.find((b) => b.id === machine.innerId);
      if (!inner) break;
      const start = inner.machines.find((m) => m.type === 'start');
      if (!start) break;
      const frame: StackFrame = {
        bodyId: state.bodyId,
        machineId: machine.id,
        repeatIndex: 0,
      };
      next = pushStack(
        { ...state, bodyId: inner.id, machineId: start.id },
        frame,
        Math.max(0, machine.power - 1),
      );
      break;
    }
  case 'left':
  case 'right':
  case 'leftWord':
  case 'rightWord':
  case 'write':
  case 'copy': {
      for (let i = 0; i < machine.power; i++) {
        applyPrimitive(machine, state.tape);
      }
      const target = findNextMachine(body, machine, state.tape);
      if (target) {
        next = { ...state, machineId: target.id };
      }
      break;
    }
  }

  return next;
}

function applyPrimitive(machine: Machine, tape: Tape): void {
  switch (machine.type) {
    case 'left':
      tape.shiftLeft();
      break;
    case 'right':
      tape.shiftRight();
      break;
    case 'leftWord':
      tape.shiftLeftWord();
      break;
    case 'rightWord':
      tape.shiftRightWord();
      break;
    case 'write':
      if (isWriteMachine(machine)) {
        tape.setSignAtHead(machine.sign);
      }
      break;
    default:
      break;
  }
}
