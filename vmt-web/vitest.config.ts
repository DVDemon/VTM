import { defineConfig } from 'vitest/config';
import path from 'node:path';

export default defineConfig({
  resolve: {
    alias: {
      '@core': path.resolve(__dirname, 'src/core'),
    },
  },
  test: {
    environment: 'node',
    include: ['tests/unit/**/*.test.ts', 'src/**/*.test.ts'],
  },
});
