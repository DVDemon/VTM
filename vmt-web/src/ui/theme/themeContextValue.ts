import { createContext } from 'react';
import type { ThemeMode, VmtThemePalette } from './vmtTheme';

export type ThemeContextValue = {
  mode: ThemeMode;
  palette: VmtThemePalette;
  setMode: (mode: ThemeMode) => void;
  toggleTheme: () => void;
};

export const ThemeContext = createContext<ThemeContextValue | null>(null);
