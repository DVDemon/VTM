import { describe, expect, it } from 'vitest';
import {
  createDebugger,
  createEmptyProject,
  getRootBody,
  stepDebugger,
} from '@core/index';

describe('Debugger', () => {
  it('steps from start along a right transition', () => {
    const project = createEmptyProject();
    const body = getRootBody(project);
    const start = body.machines.find((m) => m.type === 'start')!;
    const finish = body.machines.find((m) => m.type === 'finish')!;
    const rightId = crypto.randomUUID();
    const updated = {
      ...body,
      machines: [
        ...body.machines,
        {
          id: rightId,
          type: 'right' as const,
          center: { x: 300, y: 200 },
          size: { x: 48, y: 48 },
          power: 1,
          parentBodyId: body.id,
        },
      ],
      transitions: [
        {
          id: crypto.randomUUID(),
          startMachineId: start.id,
          finishMachineId: rightId,
          routingPoints: [],
          conditions: { enabledSigns: [' '] },
        },
        {
          id: crypto.randomUUID(),
          startMachineId: rightId,
          finishMachineId: finish.id,
          routingPoints: [],
          conditions: { enabledSigns: [' '] },
        },
      ],
    };
    const proj = {
      ...project,
      bodies: [updated],
    };
    let dbg = createDebugger(proj);
    dbg = stepDebugger(dbg);
    expect(dbg.machineId).toBe(rightId);
    dbg = stepDebugger(dbg);
    expect(dbg.tape.getHeadIndex()).toBeGreaterThan(0);
  });
});
