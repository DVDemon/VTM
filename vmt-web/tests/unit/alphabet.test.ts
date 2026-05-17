import { describe, expect, it } from 'vitest';
import { isValidAlphabet, normalizeAlphabet } from '@core/model/alphabet';
import { createProject, getRootBody } from '@core/model/factory';

describe('alphabet', () => {
  it('normalizes with lambda first and dedupes', () => {
    expect(normalizeAlphabet('10 ')).toBe(' 10');
  });

  it('createProject requires valid alphabet and names', () => {
    expect(() => createProject('', ' 01', 'M')).toThrow();
    expect(() => createProject('P', ' ', 'M')).toThrow();
    const p = createProject('My', ' 01', 'Main');
    expect(isValidAlphabet(p.alphabet)).toBe(true);
    expect(getRootBody(p).name).toBe('Main');
  });
});
