import type { ThemeMode } from './vmtTheme';

const STORAGE_KEY = 'vmt:theme';

/** Apply saved theme before first paint to reduce flash. */
export function initThemeFromStorage(): void {
  let mode: ThemeMode = 'light';
  try {
    const raw = localStorage.getItem(STORAGE_KEY);
    if (raw === 'light' || raw === 'dark') {
      mode = raw;
    } else if (window.matchMedia('(prefers-color-scheme: dark)').matches) {
      mode = 'dark';
    }
  } catch {
    /* ignore */
  }
  document.documentElement.dataset.theme = mode;
}
