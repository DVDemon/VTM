import { describe, expect, it } from 'vitest';
import { addMachine, addTransition, createProject, getRootBody, moveMachine } from '@core/model/factory';
import { routeAllTransitions, routeTransitionPolyline } from '@core/routing/transitionRoute';

describe('routeTransitionPolyline', () => {
  it('keeps path attached to ports after moving a node', () => {
    let project = createProject('P', ' 01', 'Main');
    const body = getRootBody(project);
    const start = body.machines.find((m) => m.type === 'start')!;
    const finish = body.machines.find((m) => m.type === 'finish')!;
    project = addMachine(project, body.id, 'right', { x: 300, y: 200 });
    const right = getRootBody(project).machines.find((m) => m.type === 'right')!;
    project = addTransition(project, body.id, start.id, right.id);
    project = addTransition(project, body.id, right.id, finish.id);

    const body2 = getRootBody(project);
    const before = routeAllTransitions(body2.machines, body2.transitions, 40);

    project = moveMachine(project, body2.id, right.id, { x: 500, y: 280 });
    const body3 = getRootBody(project);
    const after = routeAllTransitions(body3.machines, body3.transitions, 40);

    for (const t of body3.transitions) {
      const path = after.get(t.id)!;
      expect(path.length).toBeGreaterThanOrEqual(2);
      const startM = body3.machines.find((m) => m.id === t.startMachineId)!;
      const endM = body3.machines.find((m) => m.id === t.finishMachineId)!;
      expect(path[0]!.x).toBeGreaterThan(startM.center.x - startM.size.x);
      expect(path[path.length - 1]!.x).toBeLessThan(
        endM.center.x + endM.size.x / 2 + 2,
      );
    }

    const t0 = body3.transitions[0]!;
    expect(before.get(t0.id)!.length).toBeGreaterThan(0);
    expect(after.get(t0.id)!.length).toBeGreaterThan(0);
  });

  it('excludes endpoint machines from obstacle check', () => {
    const project = createProject('P', ' 01', 'Main');
    const body = getRootBody(project);
    const start = body.machines.find((m) => m.type === 'start')!;
    const finish = body.machines.find((m) => m.type === 'finish')!;
    const path = routeTransitionPolyline(
      body.machines,
      {
        id: 't1',
        startMachineId: start.id,
        finishMachineId: finish.id,
        routingPoints: [],
        conditions: { enabledSigns: ['0'] },
      },
      [],
      40,
    );
    expect(path.length).toBeGreaterThanOrEqual(2);
    expect(path[0]!.x).toBeGreaterThanOrEqual(start.center.x);
  });
});
