import { describe, expect, it } from 'vitest';
import {
  createDebugger,
  findBodyByName,
  formatDebuggerCallStack,
  stepDebugger,
} from '@core/index';
import { buildComplexTestProject } from '../helpers/complexProject';

describe('Debugger complex', () => {
  it('enters submachine, runs right, returns and finishes', () => {
    const project = buildComplexTestProject();
    const sub = findBodyByName(project, 'Sub')!;
    const root = project.bodies.find((b) => b.id === project.rootBodyIds[0])!;
    const complex = root.machines.find((m) => m.type === 'complex')!;

    let dbg = createDebugger(project);
    expect(dbg.bodyId).toBe(root.id);

    dbg = stepDebugger(dbg);
    expect(dbg.machineId).toBe(complex.id);

    dbg = stepDebugger(dbg);
    expect(dbg.bodyId).toBe(sub.id);
    expect(dbg.callStack.length).toBeGreaterThan(0);

    dbg = stepDebugger(dbg);
    const right = sub.machines.find((m) => m.type === 'right')!;
    expect(dbg.machineId).toBe(right.id);

    dbg = stepDebugger(dbg);
    expect(dbg.tape.getHeadIndex()).toBe(1);

    dbg = stepDebugger(dbg);
    expect(dbg.bodyId).toBe(root.id);
    expect(dbg.finished).toBe(false);

    dbg = stepDebugger(dbg);
    expect(dbg.finished).toBe(true);
  });

  it('formats call stack with current frame', () => {
    const project = buildComplexTestProject();
    let dbg = createDebugger(project);
    dbg = stepDebugger(dbg);
    dbg = stepDebugger(dbg);
    const stack = formatDebuggerCallStack(dbg);
    expect(stack.length).toBeGreaterThanOrEqual(2);
    expect(stack[stack.length - 1]!.bodyName).toBe('Sub');
  });
});
