import {
  addMachine,
  createProject,
  createSubmachine,
  findBodyByName,
  getRootBody,
  updateBody,
  type Project,
} from '../../src/core/index';

/** Main: start → complex(Sub) → finish; Sub: start → right → finish. */
export function buildComplexTestProject(): Project {
  let project = createProject('Test', ' 01', 'Main');
  project = createSubmachine(project, 'Sub');
  const sub = findBodyByName(project, 'Sub')!;
  const allSigns = sub.alphabet.split('');
  const startSub = sub.machines.find((m) => m.type === 'start')!;
  const finishSub = sub.machines.find((m) => m.type === 'finish')!;
  const rightId = crypto.randomUUID();

  project = updateBody(project, {
    ...sub,
    machines: [
      ...sub.machines,
      {
        id: rightId,
        type: 'right',
        center: { x: 300, y: 200 },
        size: { x: 48, y: 48 },
        power: 1,
        parentBodyId: sub.id,
      },
    ],
    transitions: [
      {
        id: crypto.randomUUID(),
        startMachineId: startSub.id,
        finishMachineId: rightId,
        routingPoints: [],
        conditions: { enabledSigns: allSigns },
      },
      {
        id: crypto.randomUUID(),
        startMachineId: rightId,
        finishMachineId: finishSub.id,
        routingPoints: [],
        conditions: { enabledSigns: allSigns },
      },
    ],
  });

  const subBody = findBodyByName(project, 'Sub')!;
  const root = getRootBody(project);
  project = addMachine(project, root.id, 'complex', { x: 320, y: 200 }, {
    innerBodyId: subBody.id,
  });

  const root2 = getRootBody(project);
  const rootSigns = root2.alphabet.split('');
  const complex = root2.machines.find((m) => m.type === 'complex')!;
  const start = root2.machines.find((m) => m.type === 'start')!;
  const finish = root2.machines.find((m) => m.type === 'finish')!;

  return updateBody(project, {
    ...root2,
    transitions: [
      {
        id: crypto.randomUUID(),
        startMachineId: start.id,
        finishMachineId: complex.id,
        routingPoints: [],
        conditions: { enabledSigns: rootSigns },
      },
      {
        id: crypto.randomUUID(),
        startMachineId: complex.id,
        finishMachineId: finish.id,
        routingPoints: [],
        conditions: { enabledSigns: rootSigns },
      },
    ],
  });
}
