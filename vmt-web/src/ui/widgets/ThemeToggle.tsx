import { useTheme } from '../theme/useTheme';

export function ThemeToggle() {
  const { mode, toggleTheme } = useTheme();
  const nextLabel = mode === 'light' ? 'Dark' : 'Light';

  return (
    <button
      type="button"
      className="theme-toggle"
      onClick={toggleTheme}
      title={mode === 'light' ? 'Тёмная тема' : 'Светлая тема'}
      aria-label={`Switch to ${nextLabel.toLowerCase()} theme`}
    >
      {nextLabel}
    </button>
  );
}
