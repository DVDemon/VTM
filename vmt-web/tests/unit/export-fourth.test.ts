import { describe, expect, it } from 'vitest';
import {
  addMachine,
  addTransition,
  createProject,
  getRootBody,
} from '@core/model/factory';
import { exportFourth } from '@core/export/fourth';

describe('exportFourth', () => {
  it('exports move transitions for a simple chain', () => {
    let project = createProject('P', ' 01', 'Main');
    const body = getRootBody(project);
    const start = body.machines.find((m) => m.type === 'start')!;
    const finish = body.machines.find((m) => m.type === 'finish')!;
    project = addMachine(project, body.id, 'right', { x: 300, y: 200 });
    const right = getRootBody(project).machines.find((m) => m.type === 'right')!;
    project = addTransition(project, body.id, start.id, right.id);
    project = addTransition(project, body.id, right.id, finish.id);

    const result = exportFourth(project);
    expect(result.error).toBe(false);
    expect(result.lines.length).toBeGreaterThan(0);
    expect(result.lines.some((line) => line.includes('>'))).toBe(true);
  });

  it('detects recursive complex machines by name', () => {
    let project = createProject('P', ' 01', 'Main');
    const body = getRootBody(project);
    project = addMachine(project, body.id, 'complex', { x: 300, y: 200 }, {
      complexName: 'Loop',
    });
    const cm = getRootBody(project).machines.find((m) => m.type === 'complex')!;
    const inner = project.bodies.find((b) => b.id === cm.innerId)!;
    project = addMachine(project, inner.id, 'complex', { x: 200, y: 300 }, {
      complexName: 'Loop',
    });

    const result = exportFourth(project);
    expect(result.error).toBe(true);
    expect(result.errorMessage).toContain('Recursive machine call');
  });
});
