import { describe, expect, it } from 'vitest';
import {
  addMachine,
  createEmptyProject,
  createSubmachine,
  findBodyByName,
  getRootBody,
} from '@core/index';

describe('submachine factory', () => {
  it('creates submachine body and links by innerBodyId', () => {
    let project = createEmptyProject();
    project = createSubmachine(project, 'Helper');
    const helper = findBodyByName(project, 'Helper')!;
    const root = getRootBody(project);
    project = addMachine(project, root.id, 'complex', { x: 200, y: 200 }, {
      innerBodyId: helper.id,
    });
    const node = getRootBody(project).machines.find((m) => m.type === 'complex');
    expect(node?.type).toBe('complex');
    if (node?.type === 'complex') {
      expect(node.innerId).toBe(helper.id);
      expect(node.name).toBe('Helper');
    }
    expect(project.bodies.length).toBe(2);
  });

  it('reuses existing body when complex name matches', () => {
    let project = createEmptyProject();
    project = createSubmachine(project, 'Shared');
    const before = project.bodies.length;
    const root = getRootBody(project);
    project = addMachine(project, root.id, 'complex', { x: 100, y: 100 }, {
      complexName: 'Shared',
    });
    expect(project.bodies.length).toBe(before);
  });
});
