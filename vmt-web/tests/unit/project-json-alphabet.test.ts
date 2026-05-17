import { describe, expect, it } from 'vitest';
import {
  createProject,
  deserializeProject,
  serializeProject,
} from '@core/index';

describe('project JSON alphabet', () => {
  it('round-trips project and body alphabet', () => {
    const original = createProject('Tape', ' 01', 'Main');
    const restored = deserializeProject(serializeProject(original));
    expect(restored.alphabet).toBe(' 01');
    expect(restored.bodies[0]!.alphabet).toBe(' 01');
  });

  it('normalizes alphabet on deserialize', () => {
    const original = createProject('Norm', '10 01', 'M');
    const restored = deserializeProject(serializeProject(original));
    expect(restored.alphabet).toBe(' 10');
    expect(restored.bodies[0]!.alphabet).toBe(' 10');
  });

  it('fills missing body alphabet from project', () => {
    const raw = {
      schemaVersion: 1,
      id: 'proj-1',
      name: 'Legacy',
      alphabet: ' 01',
      bodies: [
        {
          id: 'body-1',
          name: 'Root',
          machines: [],
          transitions: [],
        },
      ],
      rootBodyIds: ['body-1'],
    };
    const restored = deserializeProject(JSON.stringify(raw));
    expect(restored.alphabet).toBe(' 01');
    expect(restored.bodies[0]!.alphabet).toBe(' 01');
  });
});
