# VMT Web

Browser SPA for [Virtual Turing Machine](../README.md), built per `documentation/prompts/reasons-canvas.md`.

## Local development (NFR-013)

```bash
cd vmt-web
npm install
npm run dev
```

Open [http://localhost:5173/VTM/](http://localhost:5173/VTM/) (HashRouter + `base: /VTM/`).

## Scripts

| Command | Description |
|---------|-------------|
| `npm run dev` | Vite dev server with HMR |
| `npm run build` | Production build to `dist/` |
| `npm run preview` | Preview production build |
| `npm test` | Vitest unit tests (`@core`) |
| `npm run lint` | ESLint |

## Project files

- **`.jdtp`** — open/save via toolbar (download to disk; binary codec port in progress).
- **`.vmt.json`** — native JSON format; optional draft in `localStorage` (prefix `vmt:`).
- Binary `.jdtp` is **not** stored in `localStorage`.

## Structure

See `documentation/prompts/reasons-canvas.md` § Structure — `src/core` (domain), `src/ui`, `src/storage`.
