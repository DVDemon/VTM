import { describe, expect, it } from 'vitest';
import { runCompilerChecks, createSubmachine } from '@core/index';
import { buildComplexTestProject } from '../helpers/complexProject';

describe('compiler checks complex', () => {
  it('passes on valid complex tree', () => {
    const project = buildComplexTestProject();
    expect(runCompilerChecks(project)).toEqual([]);
  });

  it('reports missing inner reference', () => {
    const project = buildComplexTestProject();
    const root = project.bodies.find((b) => b.id === project.rootBodyIds[0])!;
    const complex = root.machines.find((m) => m.type === 'complex')!;
    if (complex.type !== 'complex') return;
    const broken = {
      ...project,
      bodies: project.bodies.map((b) =>
        b.id === root.id
          ? {
              ...b,
              machines: b.machines.map((m) =>
                m.id === complex.id ? { ...m, innerId: 'missing-id' } : m,
              ),
            }
          : b,
      ),
    };
    const errors = runCompilerChecks(broken);
    expect(errors.some((e) => e.message.includes('missing submachine'))).toBe(
      true,
    );
  });

  it('reports unreferenced submachine body', () => {
    let project = buildComplexTestProject();
    project = createSubmachine(project, 'Orphan');
    const errors = runCompilerChecks(project);
    expect(errors.some((e) => e.message.includes('not referenced'))).toBe(
      true,
    );
  });
});
