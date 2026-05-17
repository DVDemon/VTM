import { describe, expect, it } from 'vitest';
import {
  defaultTransitionConditions,
  formatTransitionConditions,
  pointOnPolyline,
  resolveConditionsAnchor,
  resolveLabelT,
} from '@core/model/transitionLabel';

describe('transitionLabel', () => {
  it('formats single sign and star', () => {
    const alphabet = ' 01';
    expect(
      formatTransitionConditions(alphabet, { enabledSigns: ['0'] }),
    ).toBe('0');
    expect(
      formatTransitionConditions(alphabet, { enabledSigns: ['0', '1'] }),
    ).toBe('*');
  });

  it('default conditions enable full alphabet', () => {
    const c = defaultTransitionConditions(' 01');
    expect(c.enabledSigns).toEqual([' ', '0', '1']);
  });

  it('label position follows path when geometry changes', () => {
    const transition = { conditionsLabelT: 0.5 };
    const pathA = [
      { x: 0, y: 0 },
      { x: 100, y: 0 },
    ];
    const pathB = [
      { x: 0, y: 40 },
      { x: 100, y: 40 },
    ];
    const a = resolveConditionsAnchor(transition, pathA);
    const b = resolveConditionsAnchor(transition, pathB);
    expect(a.x).toBeCloseTo(50);
    expect(b.x).toBeCloseTo(50);
    expect(b.y).toBeCloseTo(40);
    expect(resolveLabelT(transition, pathA)).toBe(0.5);
  });

  it('interpolates along polyline', () => {
    const mid = pointOnPolyline(
      [
        { x: 0, y: 0 },
        { x: 100, y: 0 },
      ],
      0.5,
    );
    expect(mid.x).toBeCloseTo(50);
    expect(mid.y).toBeCloseTo(0);
  });
});
