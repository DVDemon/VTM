import { describe, expect, it } from 'vitest';
import { decodeJdtp, encodeJdtp } from '../../src/core/serializer/jdtp-io';
import { buildComplexTestProject } from '../helpers/complexProject';
import { findBodyByName } from '@core/index';

describe('jdtp complex', () => {
  it('round-trips multi-body project via JSON-in-jdtp', () => {
    const original = buildComplexTestProject();
    const buffer = encodeJdtp(original);
    const restored = decodeJdtp(buffer);
    expect(restored.bodies.length).toBe(2);
    expect(findBodyByName(restored, 'Sub')).toBeDefined();
  });
});
