import { describe, expect, it } from 'vitest';
import { runCompilerChecks } from '@core/checks/compilerChecks';
import {
  addMachine,
  addTransition,
  createProject,
  getRootBody,
  updateTransitionConditions,
} from '@core/model/factory';

function signCoverageErrors(project: ReturnType<typeof createProject>) {
  return runCompilerChecks(project).filter((e) =>
    e.message.includes('No outgoing transition for signs'),
  );
}

describe('runCompilerChecks — outgoing signs', () => {
  it('does not flag when track symbols are fully covered via *', () => {
    let project = createProject('P', ' 01', 'Main');
    const body = getRootBody(project);
    const start = body.machines.find((m) => m.type === 'start')!;
    const finish = body.machines.find((m) => m.type === 'finish')!;
    project = addMachine(project, body.id, 'right', { x: 300, y: 200 });
    const right = getRootBody(project).machines.find((m) => m.type === 'right')!;
    project = addTransition(project, body.id, start.id, right.id);
    project = addTransition(project, body.id, right.id, finish.id);

    const body2 = getRootBody(project);
    const t = body2.transitions.find((tr) => tr.startMachineId === start.id)!;
    project = updateTransitionConditions(project, body2.id, t.id, {
      enabledSigns: ['0', '1'],
    });

    expect(signCoverageErrors(project)).toHaveLength(0);
  });

  it('does not flag default transition (all alphabet, like desktop)', () => {
    let project = createProject('P', ' 01', 'Main');
    const body = getRootBody(project);
    const start = body.machines.find((m) => m.type === 'start')!;
    const finish = body.machines.find((m) => m.type === 'finish')!;
    project = addMachine(project, body.id, 'right', { x: 300, y: 200 });
    const right = getRootBody(project).machines.find((m) => m.type === 'right')!;
    project = addTransition(project, body.id, start.id, right.id);
    project = addTransition(project, body.id, right.id, finish.id);

    expect(signCoverageErrors(project)).toHaveLength(0);
  });

  it('flags missing signs when coverage is partial', () => {
    let project = createProject('P', ' 01', 'Main');
    const body = getRootBody(project);
    const start = body.machines.find((m) => m.type === 'start')!;
    const finish = body.machines.find((m) => m.type === 'finish')!;
    project = addMachine(project, body.id, 'right', { x: 300, y: 200 });
    const right = getRootBody(project).machines.find((m) => m.type === 'right')!;
    project = addTransition(project, body.id, start.id, right.id);
    project = addTransition(project, body.id, right.id, finish.id);

    const body2 = getRootBody(project);
    const t = body2.transitions.find((tr) => tr.startMachineId === start.id)!;
    project = updateTransitionConditions(project, body2.id, t.id, {
      enabledSigns: ['0'],
    });

    const errs = signCoverageErrors(project);
    expect(errs.length).toBeGreaterThan(0);
    expect(errs[0]!.message).toContain("signs '");
  });
});
