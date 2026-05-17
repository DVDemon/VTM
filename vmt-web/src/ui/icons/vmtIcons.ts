import type { ThemeMode } from '../theme/vmtTheme';

/** Resolve static asset under Vite `base` (e.g. `/VTM/`). */
export function assetUrl(relativePath: string): string {
  const base = import.meta.env.BASE_URL;
  const path = relativePath.replace(/^\//, '');
  return `${base}${path}`;
}

/** Diagram / light panels: dark glyphs; dark diagram: light glyphs (desktop VmtIcons). */
export function pickThemedAsset(
  mode: ThemeMode,
  lightSurfacePath: string,
  darkSurfacePath: string,
): string {
  return assetUrl(mode === 'dark' ? darkSurfacePath : lightSurfacePath);
}

const machineStartLight = 'icons/machines/icon_start_black.png';
const machineStartDark = 'icons/machines/icon_start.png';
const machineFinishLight = 'icons/machines/icon_finish_black.png';
const machineFinishDark = 'icons/machines/icon_finish.png';

/** Start/finish node on the diagram canvas. */
export function machineStartFinishIconUrl(
  mode: ThemeMode,
  type: 'start' | 'finish',
): string {
  if (type === 'start') {
    return pickThemedAsset(mode, machineStartLight, machineStartDark);
  }
  return pickThemedAsset(mode, machineFinishLight, machineFinishDark);
}

/** Colored icons on the editor toolbar chrome (always light glyphs on blue). */
export function editorToolbarMachineIconUrl(type: 'start' | 'finish'): string {
  return assetUrl(
    type === 'start' ? machineStartDark : machineFinishDark,
  );
}

export const debugIcons = {
  run: assetUrl('icons/debug/run.gif'),
  step: assetUrl('icons/debug/step.gif'),
  stop: assetUrl('icons/debug/stop.gif'),
  pause: assetUrl('icons/debug/pause.gif'),
  stepOver: assetUrl('icons/debug/step_over.gif'),
} as const;

/** Main toolbar: open debugger / run (desktop `icon_run.gif`). */
export const mainRunIconUrl = assetUrl('icons/main/icon_run.gif');
