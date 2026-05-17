# ADR-007: Деплой на GitHub Pages — HashRouter, base path, Actions

Дата: 2026-05-17

Статус: Accepted

## Контекст

Бандл VMT Web публикуется на GitHub Pages по адресу `https://<user>.github.io/VMT/`. Особенности:

- GH Pages раздаёт статику, не умеет fallback на `index.html` для произвольных путей (history-API маршрутизация ломается при обновлении страницы);
- проект публикуется не в корне домена, а в подкаталоге `/VMT/`;
- деплой делается через GitHub Actions (`actions/deploy-pages`).

Рассматривались: HashRouter vs BrowserRouter с workaround `404.html`; deploy через `gh-pages` npm-пакет vs официальный action.

Требования: NFR-001, NFR-010, FR-001 (deep-link на конкретный экран).

## Решение

- **Маршрутизация:** `HashRouter` из `react-router-dom`. URL вида `https://<user>.github.io/VMT/#/editor`. Не зависит от серверного fallback, корректно работает при F5.
- **Base path:** в `vite.config.ts` фиксируется `base: '/VMT/'`. Все ассеты (иконки, JSON упражнений) запрашиваются через `import` или `new URL(..., import.meta.url)`.
- **CI/CD:** GitHub Actions workflow `ci.yml`:
  1. `pnpm install --frozen-lockfile` (или `npm ci`);
  2. `npm run lint && npm run test && npm run build`;
  3. `actions/upload-pages-artifact` → `actions/deploy-pages` (на `main`).
- **Артефакт деплоя:** содержимое `dist/`. Перед публикацией копируется `dist/index.html → dist/404.html` (мера предосторожности для возможных deep-link через BrowserRouter в будущем).
- **Кэширование:** `Cache-Control` управляется GH Pages, имена файлов с хэшем (`*.[hash].js`) — даёт безопасный долгий кэш для всего, кроме `index.html`.
- **Версия Node** — текущий LTS, зафиксирован в `package.json` через `engines` и в Actions через `actions/setup-node@v4`.

## Альтернативы

### Альтернатива 1: BrowserRouter + 404.html-redirect

Распространённый workaround (skipping `404.html` копирует логику восстановления path в `sessionStorage`). Работает, но:

- ломает прямые открытия глубоких ссылок (мигание, потеря якоря);
- усложняет конфигурацию;
- не даёт ощутимой пользы — URL с `#` для приложения-инструмента приемлемы.

### Альтернатива 2: Netlify / Vercel

Поддерживают SPA fallback из коробки, но добавляют ещё одного провайдера и противоречат явному требованию пользователя «GitHub Pages».

### Альтернатива 3: Деплой через `gh-pages` npm-пакет

Работает, но требует `GITHUB_TOKEN`/Personal Access Token и push в ветку. Официальный `actions/deploy-pages` интегрирован с правами `pages: write`, не требует токенов вручную, и поддерживает «Environment» с защитой.

## Последствия

Положительные:

- Простой и предсказуемый деплой статикой без серверного fallback.
- HashRouter работает на любом статическом хостинге без настроек.
- CI-pipeline стандартный, легко переиспользуется.

Отрицательные / компромиссы:

- URL содержат `#`. Это эстетический минус, но не функциональный.
- Деплой только из `main` → нет staging-окружения в V1. Митигация: PR-превью через Vercel/Netlify (опционально, V1.1).
- При переименовании репозитория надо обновить `base` в `vite.config.ts`.

## Проверка соблюдения

- В `vite.config.ts` есть `base: '/VMT/'`.
- В `src/main.tsx` используется `HashRouter`, а не `BrowserRouter`.
- В `.github/workflows/ci.yml` есть джобы `build` и `deploy` с `actions/deploy-pages`.
- В корне репозитория есть файл `.nojekyll` (или Action его добавляет в `dist/`), чтобы GH Pages не пытался Jekyll-процессить ассеты с `_`-префиксом.
- Sanity-проверка деплоя: после merge в `main` страница доступна, в DevTools все ассеты тянутся с `https://<user>.github.io/VMT/...` (нет 404).
- В Settings → Pages включён source = «GitHub Actions».
