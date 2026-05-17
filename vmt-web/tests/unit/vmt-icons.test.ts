import { describe, expect, it } from 'vitest';
import {
  assetUrl,
  editorToolbarMachineIconUrl,
  machineStartFinishIconUrl,
  pickThemedAsset,
} from '../../src/ui/icons/vmtIcons';

describe('vmtIcons', () => {
  it('prefixes paths with vite base', () => {
    expect(assetUrl('icons/debug/run.gif')).toMatch(/icons\/debug\/run\.gif$/);
  });

  it('picks dark glyphs on light diagram and light glyphs on dark diagram', () => {
    expect(pickThemedAsset('light', 'icons/a_black.png', 'icons/a.png')).toContain(
      'a_black',
    );
    expect(pickThemedAsset('dark', 'icons/a_black.png', 'icons/a.png')).toContain(
      'icons/a.png',
    );
  });

  it('maps start/finish for canvas and toolbar', () => {
    expect(machineStartFinishIconUrl('light', 'start')).toContain('icon_start_black');
    expect(machineStartFinishIconUrl('dark', 'finish')).toContain('icon_finish.png');
    expect(editorToolbarMachineIconUrl('start')).toContain('icon_start.png');
    expect(editorToolbarMachineIconUrl('finish')).toContain('icon_finish.png');
  });
});
