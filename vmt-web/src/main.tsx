import { StrictMode } from 'react';
import { createRoot } from 'react-dom/client';
import App from './App';
import { initThemeFromStorage } from './ui/theme/initTheme';
import './index.css';

initThemeFromStorage();

createRoot(document.getElementById('root')!).render(
  <StrictMode>
    <App />
  </StrictMode>,
);
