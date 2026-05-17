import { describe, expect, it } from 'vitest';
import {
  createEmptyProject,
  deserializeProject,
  serializeProject,
} from '@core/index';

describe('project JSON', () => {
  it('round-trips empty project', () => {
    const p = createEmptyProject('Test');
    const json = serializeProject(p);
    const back = deserializeProject(json);
    expect(back.name).toBe('Test');
    expect(back.bodies.length).toBe(1);
    expect(back.rootBodyIds[0]).toBe(back.bodies[0]!.id);
  });
});
