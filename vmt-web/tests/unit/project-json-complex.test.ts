import { describe, expect, it } from 'vitest';
import {
  deserializeProject,
  serializeProject,
} from '../../src/core/serializer/json';
import { buildComplexTestProject } from '../helpers/complexProject';
import { findBodyByName } from '@core/index';

describe('project JSON complex', () => {
  it('round-trips multi-body project with complex innerId', () => {
    const original = buildComplexTestProject();
    const raw = serializeProject(original);
    const restored = deserializeProject(raw);
    expect(restored.bodies.length).toBe(original.bodies.length);
    const sub = findBodyByName(restored, 'Sub');
    expect(sub).toBeDefined();
    const root = restored.bodies.find((b) => b.id === restored.rootBodyIds[0])!;
    const complex = root.machines.find((m) => m.type === 'complex');
    expect(complex?.type).toBe('complex');
    if (complex?.type === 'complex') {
      expect(complex.innerId).toBe(sub!.id);
    }
  });
});
