import { defineConfig } from 'vite';
import react from '@vitejs/plugin-react';
import path from 'node:path';

export default defineConfig({
  plugins: [react()],
  base: '/VTM/',
  resolve: {
    alias: {
      '@core': path.resolve(__dirname, 'src/core'),
    },
  },
  build: {
    target: 'es2020',
  },
  server: {
    port: 5173,
  },
});
