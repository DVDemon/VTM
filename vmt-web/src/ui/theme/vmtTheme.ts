export type ThemeMode = 'light' | 'dark';

export type VmtThemePalette = {
  readonly primary: string;
  readonly primaryDark: string;
  readonly background: string;
  readonly surface: string;
  readonly diagramBackground: string;
  readonly diagramGrid: string;
  readonly diagramLine: string;
  readonly textOnDark: string;
  readonly textOnLight: string;
  readonly error: string;
  readonly machineFill: string;
  readonly machineFillSelected: string;
  readonly machineFillHover: string;
  readonly linkPreview: string;
};

export const lightTheme: VmtThemePalette = {
  primary: '#1976D2',
  primaryDark: '#1565C0',
  background: '#012C40',
  surface: '#0D3B52',
  diagramBackground: '#FAFAFA',
  diagramGrid: '#E0E0E0',
  diagramLine: '#212121',
  textOnDark: '#FFFFFF',
  textOnLight: '#212121',
  error: '#D32F2F',
  machineFill: '#FFFFFF',
  machineFillSelected: '#BBDEFB',
  machineFillHover: '#E3F2FD',
  linkPreview: '#1976D2',
};

export const darkTheme: VmtThemePalette = {
  primary: '#42A5F5',
  primaryDark: '#1976D2',
  background: '#012C40',
  surface: '#0D3B52',
  diagramBackground: '#263238',
  diagramGrid: '#455A64',
  diagramLine: '#ECEFF1',
  textOnDark: '#FFFFFF',
  textOnLight: '#ECEFF1',
  error: '#EF5350',
  machineFill: '#37474F',
  machineFillSelected: '#1565C0',
  machineFillHover: '#455A64',
  linkPreview: '#42A5F5',
};

/** @deprecated Use `useTheme().palette` */
export const vmtTheme = lightTheme;

export function getThemePalette(mode: ThemeMode): VmtThemePalette {
  return mode === 'dark' ? darkTheme : lightTheme;
}
