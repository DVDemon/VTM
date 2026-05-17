# REASONS Canvas — V1 веб-версии VMT

## Метаданные

- Название изменения: Первая версия веб-приложения «VMT Web» (SPA на GitHub Pages).
- Связанные требования: FR-001…FR-023, NFR-001…NFR-013 (см. [`analysis.md`](analysis.md)).
- Связанные ADR: ADR-001 … ADR-007 (см. `documentation/adr/`).
- Связанная архитектура: `documentation/workspace.dsl` (по шаблону `templates/workspace.dsl`).
- Базовый источник требований: десктоп VMT (`documentation/*.md`).

## R — Requirements

- Конечный пользователь — **студент / преподаватель**, открывает SPA по адресу `https://<user>.github.io/VMT/` и без регистрации создаёт проект машины Тьюринга, редактирует диаграмму, запускает шаги/непрерывный прогон, видит ленту, экспортирует результат (PNG, PlantUML, четвёрки).
- В V1 входит **полный функциональный паритет** с десктопом (см. FR-001…FR-023): редактор + интерпретатор + проверки + упражнения + Undo + три экспорта + открытие/сохранение диаграмм в формате **`.jdtp` через локальный файловый диалог скачивания и загрузки** (без удалённого хранилища и без сохранения бинарных `.jdtp` в браузерном key-value-хранилище).
- **Локальное развертывание для отладки** (NFR-013): разработчик поднимает приложение командой `npm install && npm run dev` в каталоге веб-проекта; по умолчанию открывается `http://localhost:5173` (или следующий свободный порт Vite). В README описаны переменные окружения для базового URL при необходимости; горячая перезагрузка включена.
- Все данные **не отправляются на сервер приложения**. Черновики проекта могут кэшироваться в браузере как строка `.vmt.json` в localStorage (опционально); файлы `.jdtp` живут только на диске пользователя после явного «Сохранить / Открыть». Нет бэкенда, нет аутентификации, нет передачи содержимого проектов по сети после первой загрузки бандла.
- Сборка — статический `vite build`, деплой на ветку `gh-pages` через GitHub Actions. Маршрутизация — `HashRouter` + `base: '/VMT/'`.
- Поддерживаемые браузеры: последние две мажорные версии Chrome/Firefox/Safari (см. NFR-004).
- Производительность: при 200 узлах и 400 связях drag — ≥ 30 FPS; стартовый бандл ≤ 350 KB gzip.
- Локализация RU/EN, включая тексты упражнений (`exercises.xml` мигрируется в JSON).
- Не входит в V1: облачная синхронизация, OAuth, совместное редактирование, мобильное редактирование, сохранение черновика проекта > 4 MB в localStorage без явного экспорта файла на диск (используются «Export project as .json» / «Save .jdtp» на диск). Полный список — §10 в `analysis.md`.

## E — Entities

- `Project` — корневой контейнер: имя, алфавит, список верхнеуровневых `ComplexMachine`, `schemaVersion`.
- `Alphabet` — упорядоченная строка символов; первый символ = λ (пустая ячейка).
- `Machine` (полиморф) — узел диаграммы: `MT_START | MT_FINISH | MT_LEFT | MT_RIGHT | MT_LEFT_WORD | MT_RIGHT_WORD | MT_WRITE | MT_COPY | MT_COMPLEX`. Поля: `id`, `type`, `center`, `size`, `power`, `parentId`.
- `AlphaMachine` (`MT_WRITE`) — расширение с полем `sign`.
- `ComplexMachine` — узел-контейнер, ссылается на разделяемое `MachineBody` по `innerId` (поддерживает переиспользование машины в нескольких местах).
- `MachineBody` — собственно «тело» комплексной машины: `machines[]`, `transitions[]`, `alphabet`.
- `Transition` — связь между двумя узлами: `startMachineId`, `finishMachineId`, ломаная `routingPoints[]`, `conditions`, точка/сегмент подписи.
- `TransitionConditions` — множество включённых символов алфавита; есть `isAllEnabled` (для `*` в PlantUML).
- `Tape` (runtime) — бесконечная лента блоками по 10 символов, `headIndex`, `zeroIndex` (порт `VMTLine`).
- `DebuggerState` (runtime) — `currentMachineId`, `currentScopeId`, `callStack: StackFrame[]`, `isFinished`.
- `Exercise` — `id`, `text_en/text_ru`, `alphabet`, `tests: {input, output}[]`, `completed`.
- `UndoStack` — снапшоты `Project` (immer-style structural sharing).

## A — Approach

- **Доменное ядро отдельно от UI.** Каталог `@core/*` — чистый TypeScript без React/konva. Это:
  - даёт юнит-тесты без DOM;
  - облегчает порт алгоритмов (`Pathfinder`, `DiagramBezier`, `VMTDebuger`, `VMTSerializer`, `VmtExportPlantUml`, `VMTExport4th`) практически 1-в-1 с C++ — с заменой `QPoint/QRect` на простые value-объекты.
- **Иммутабельная модель.** Любая правка диаграммы возвращает новый `Project`. Undo/Redo — стек ссылок. Это даёт детерминированный диф для перерисовки konva-слоёв.
- **react-konva как сцена.** Разделение слоёв: фон+сетка, рёбра (Path2D из Bézier), узлы, overlay (выделение/анимация шага). Hit-test связей через flatten-полилинию.
- **localStorage с дисциплиной размера.** Хранится только компактное состояние и черновики в виде строк **`.vmt.json`** (не бинарный `.jdtp`). Ограничение ~4 MB на ключ; при превышении — предложение сохранить на диск (`.vmt.json` или `.jdtp`). Список недавних — метаданные (имя пути / id), без больших blob.
- **A\* в основном потоке** (V1). Если в нагрузочных тестах окажется тормозом — выносим в Web Worker (V1.1).
- **Деплой по push в main.** GitHub Actions: lint + test + build + `actions/deploy-pages`. Без preview-стейджа.
- **Файловый обмен `.jdtp`** — модуль `@core/serializer/jdtp-io.ts`: декодирует и кодирует бинарный поток **идентичный десктопному `QDataStream`** (импорт с диска / экспорт «Сохранить как…» через `<a download>` или File System Access API где доступно). В **localStorage бинарные `.jdtp` не сохраняются**. Тесты — round-trip на эталонных файлах из C++ и эталонных fixture-буферах.

## S — Structure

```
vmt-web/
├── public/                       # static (favicon, base manifest)
├── src/
│   ├── core/                     # @core — без UI
│   │   ├── model/                # Project, Machine, Transition, Alphabet
│   │   ├── tape/                 # Tape (порт VMTLine)
│   │   ├── interpreter/          # Debugger, FindNextMachine
│   │   ├── checks/               # compiler_checks
│   │   ├── routing/              # Pathfinder (A*), DiagramBezier
│   │   ├── serializer/
│   │   │   ├── json.ts
│   │   │   ├── jdtp-io.ts           # чтение/запись бинарного .jdtp (совместимость с десктопом), только через файловый API UI
│   │   ├── export/
│   │   │   ├── plantuml.ts
│   │   │   ├── fourth.ts         # «четвёрки»
│   │   │   └── png.ts            # OffscreenCanvas wrapper
│   │   └── undo/                 # snapshot-based UndoStack
│   ├── storage/                  # React Context + localStorage
│   ├── ui/
│   │   ├── App.tsx               # HashRouter, layout
│   │   ├── screens/
│   │   │   ├── MainScreen.tsx
│   │   │   ├── EditorScreen.tsx
│   │   │   ├── CompilerScreen.tsx
│   │   │   ├── DebuggerScreen.tsx
│   │   │   ├── ExercisesScreen.tsx
│   │   │   └── Export4thScreen.tsx
│   │   ├── canvas/               # react-konva-сцена
│   │   ├── widgets/              # toolbar, alphabet editor, tape view, conditions editor
│   │   └── theme/                # VmtTheme (Material Blue)
│   ├── i18n/                     # RU/EN ресурсы
│   └── main.tsx
├── tests/
│   ├── unit/                     # Vitest на @core
│   ├── fixtures/                 # эталонные .jdtp + ожидаемые .json/.puml
│   └── e2e/                      # Playwright
├── documentation/                # (этот каталог; уже в репо)
├── .github/workflows/ci.yml
├── vite.config.ts                # base: '/VMT/'
├── tsconfig.json
└── package.json
```

- **API ядра** экспортируется через barrel `@core/index.ts`; UI знает только эти типы и функции, никаких прямых импортов внутренних модулей.
- **Конфигурация Vite:** `base: '/VMT/'`, `build.target: 'es2020'`, code-splitting по экранам.

## O — Operations

1. Создать `documentation/adr/` и положить туда **ADR-001 … ADR-007** по шаблону (`templates/adr-template.md`).
2. Создать `documentation/workspace.dsl` по шаблону `templates/workspace.dsl`: persons, system, контейнеры (`spa`, `editor`, `core`, `storage`, `exercises_resource`), компоненты `core`, связи, deployment view `Production` (GitHub Pages + Browser), dynamic views для open-project / step / export-plantuml.
3. Поднять каркас Vite + React + TS, ESLint/Prettier, Vitest, Playwright. Smoke-test: `npm test` зелёный, `npm run build` собирает; **`npm run dev` запускает локальный сервер для отладки** (см. NFR-013).
4. Настроить GitHub Actions: lint → test → build → deploy на `gh-pages`. Smoke: после merge в main страница доступна.
5. Реализовать иммутабельную доменную модель (`Project`, `Machine`, `Transition`, `Alphabet`) + типы. Тест: round-trip JSON.
6. Реализовать `Tape` (бесконечная лента, блоки по 10). Тест: shift влево/вправо за границы → массив расширяется, λ заполняется.
7. Реализовать `Debugger.step()` со стеком и FINISH-логикой. Тест: программа «двигаться вправо до λ» завершается на правильной позиции.
8. Реализовать `compiler_checks` (5 правил). Тест: каждая ошибка детектируется на минимальном проекте.
9. Реализовать сериализатор JSON. Тест: семантическое равенство до/после.
10. Реализовать **импорт и экспорт `.jdtp` через файловый диалог** (`input[type=file]` + `Blob`/`download`): парсер и сериализатор бинарного потока v1+v2 в `@core`. Тест: round-trip fixture ↔ модель ↔ fixture-байты.
11. Реализовать A\*-pathfinder. Тест: 12 сценариев (как в `tests/pathfinder/`).
12. Реализовать Безье-рендерер (`buildConnectorPath`, `flattenPath`, `isPointNearStroke`).
13. Собрать каркас react-konva сцены с фоном/сеткой/слоями. Smoke: пустой проект рисует пустую сетку.
14. Реализовать перемещение/добавление/удаление узлов + drag-create связи.
15. Реализовать редактор условий перехода.
16. Реализовать экраны Main/Editor/Compiler/Debugger/Exercises/Export4th с тулбаром.
17. Реализовать экспорты: PNG → `OffscreenCanvas` + download; PlantUML; четвёрки.
18. Реализовать Undo/Redo через стек снапшотов `Project`.
19. Локализация (i18next, RU/EN), переключатель в шапке.
20. (Опц., V1.1) Service Worker через Workbox.
21. Acceptance: вручную пройти UC-1…UC-7 на трёх фикстурных проектах. Playwright smoke зелёный.

## N — Norms

- Не добавлять зависимости без причины; каждая третья зависимость — отдельный пункт в PR (или ADR).
- Публичный API `@core` имеет полный TypeScript-тип и не содержит `any`.
- Любое изменение публичного API `@core` или формата JSON-проекта → обновить ADR/CHANGELOG **до** изменения кода.
- Любое изменение поведения, видимое пользователю → обновить требование (FR-/NFR-) и Canvas в той же PR.
- Тесты на ядро запускаются в CI обязательно; PR без зелёного CI не мерджится.
- В каждом PR — ≤ 400 изменённых строк; крупные изменения делятся.
- Структура папок и алиасов фиксируется в `tsconfig.paths` и не меняется без ADR.
- Все prompt-артефакты — в `documentation/prompts/`; правки требований — в `documentation/`.
- Стиль: Prettier (по умолчанию), ESLint с правилами `@typescript-eslint/recommended` + `react-hooks`.
- Коммиты — Conventional Commits (`feat:`, `fix:`, `refactor:`, `docs:`, `test:`, `chore:`).

## S — Safeguards

- Не отправлять данные пользователя на сервер; не подключать аналитику в V1 (приватность).
- CSP запрещает inline-скрипты и удалённые домены, кроме самого GH Pages.
- Не хранить в localStorage ничего, что выглядит как PII; все ключи — под префиксом `vmt:`.
- При попытке сохранить черновик > 4 MB в localStorage — предупреждение + предложение сохранить на диск (`.vmt.json` или `.jdtp`).
- Импорт/экспорт `.jdtp` только через действия пользователя на диске; **никогда не отправлять содержимое `.jdtp` на удалённый сервер**.
- Парсер/писатель `.jdtp` обрабатывает ошибки без падения UI (`try/catch`, человекочитаемое сообщение).
- Экспорт «четвёрок» прерывается с явной ошибкой при обнаружении рекурсии (как в десктопе) — не давать молчаливо некорректный результат.
- При смене JSON-схемы — обязательная миграция и тест round-trip на эталонных фикстурах предыдущей версии.
- Никакие секреты (включая GitHub-токены для деплоя) не попадают в репозиторий; используем `secrets.GITHUB_TOKEN` в Actions.
- Если выполняется рефакторинг без изменения поведения — синхронизировать Canvas сразу после изменения кода.
- Если функция меняет публичный API ядра — Canvas и ADR обновляются **до** генерации кода.
