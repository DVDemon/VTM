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
- `EditorTool` — режим взаимодействия с холстом: `pointer | link | pan | delete | start | finish | left | right | leftWord | rightWord | write | copy | complex` (порт `vmtactions/*` и `tool_*` из `formeditormachine.ui`).
- `EditorToolbarState` — активный `EditorTool`, выделенный объект (`machine` / `transition`), масштаб холста (`zoom` 25…200 %), смещение вида (`viewOffset`) для pan, черновик связи `LinkDraft { startMachineId, cursor, hoverMachineId? }`.
- `EditorInspector` — боковая панель свойств выделенного узла Write (λ) или перехода.

### Поведение инструментов редактора (FR-003 / FR-006)

| Инструмент | Поведение |
|---|---|
| **pointer** | Выделение узла или связи; **drag узла** — перемещается `Group` (прямоугольник + подпись S/F/L/R/…); **все входящие и исходящие связи** пересчитываются и рисуются на каждом кадре (`routeAllTransitions` + `machineDrag` preview); по `dragEnd` — snap к сетке и commit в модель. Перетаскивание подписи связи вдоль линии. |
| **link** | 1-й клик по узлу с исходящим портом → начало связи, динамическая линия до курсора; 2-й клик по узлу с входным портом → создаётся `Transition`, маршрут пересчитывается (A\*). Пока черновик активен — предпросмотр «резиновой» линии. |
| **pan** | Перетаскивание холста (`viewOffset`, как `VMTActionTranslate`): прозрачный слой перехватывает drag; узлы **не** двигаются. Средняя кнопка — pan при любом инструменте. |
| **pointer + drag по фону** | Drag только по пустому полю (не по узлу) сдвигает вид; по узлу — перемещение узла (редактор). |
| **Debugger (`navigate`)** | Как десктоп `FormDebugerMachine` + `VMTActionTranslate`: только pan, узлы не перетаскиваются; канвас на всё поле под лентой. |
| **Лента в отладчике** | `TapeView`: число ячеек = `tapeCellCountForWidth` (ResizeObserver на всю ширину экрана, `flex:1` на ячейку). Под ячейкой — **индекс относительно 0**. Ввод символов алфавита; клик — головка. Окно `tapeViewStart`; `« ‹ ⌂ › »` (страница = ширина окна); `alignTapeViewToHead`. |
| **Export 4th** | Экран `/export-4th`: порт `VMTExport4th` → `exportFourth()`; настраиваемые delimiter/left/right/stop/quotes; текст в `textarea` + **Copy to clipboard** (`navigator.clipboard`). Проверка рекурсии complex по имени. |
| **delete** | Клик по узлу или связи — немедленное удаление (без предварительного выделения). |
| **start…complex** | Клик по пустой клетке сетки — создание узла; **инструмент остаётся активным** (можно ставить несколько узлов подряд, как в десктопе). |
| **Повторное применение** | После действия на холсте активный инструмент **не сбрасывается** на pointer (в т.ч. link — можно провести несколько связей подряд). |
| **Подписи узлов** | Короткий текст внутри прямоугольника (S, F, L, R, λ/символ для Write, C, M); размер шрифта подгоняется под `size`. |
| **Write (λ) при выделении** | `EditorInspector`: выбор символа из алфавита проекта (включая λ). |
| **Связь при выделении (pointer)** | `EditorInspector`: мультивыбор символов алфавита для условий; метка на линии (`*` если все); подпись перетаскивается вдоль коннектора. |
| **Submachine (FR-004/005)** | `Project.bodies[]` + `ComplexMachine.innerId` (общее тело, как `VMTComplexMachineInner`). **Создание:** `createSubmachine`, инструмент complex → `SubmachinePanel` (New / Existing, список тел, `+ New`). **Размещение:** ссылка на существующее тело (`innerBodyId`) или новое по имени. **Редактор:** `BodyBreadcrumb`, двойной клик по узлу complex → внутренняя диаграмма. **Отладчик:** вход/выход со стеком (`push` return + `power-1` inner, как `VMTDebuger`); канвас по `dbg.bodyId`; панель **Stack**. **Check:** рекурсивный обход inner; ошибки missing `innerId`, orphan body. |

### Создание проекта и машины (FR-001 / FR-002)

- **Новый проект** — модальный диалог `NewProjectDialog`: обязательны **имя проекта**, **имя корневой машины**, **алфавит** (нормализация: λ первым, без дубликатов; минимум 2 символа, как `FormNewProject`). Кнопка Create неактивна, пока поля невалидны.
- **Узел MT_COMPLEX** — `SubmachinePanel`: новая подмашина по имени, выбор существующей из списка, или `+ New`; клик по сетке размещает узел; двойной клик по узлу открывает внутреннюю диаграмму.

### Подпись связи и размер холста

- Позиция метки хранится как **`conditionsLabelT`** (0…1 вдоль полилинии коннектора). При перестроении маршрута (перемещение узлов) метка **сохраняет относительное положение** на линии, в том числе после ручного перетаскивания пользователем.
- **Холст редактора** занимает всё видимое поле под панелью действий: `editor-canvas-wrap` — flex:1 на оставшуюся высоту окна; Stage по `useElementSize` на весь контейнер. `EditorInspector` / `SubmachinePanel` — **оверлей** поверх канваса (правый верхний угол), не отъедают ширину диаграммы.
- **Перестроение связей** при перемещении узла: `routeAllTransitions` пересчитывает все переходы; узлы-концы перехода **не считаются препятствиями** для A\*; учитываются уже проложенные пути (избежание пересечений).
- **Live preview при drag узла** (`DiagramStage`): состояние `machineDrag` подменяет `center` перетаскиваемой машины в `layoutMachines`; связи (входящие/исходящие) и подписи условий перерисовываются до отпускания кнопки мыши — как `VMTActionPointer::Drag` / `BeginDrag` в десктопе.
- **Экспорт «четвёрок»** (`exportFourth`): нумерация состояний (`MapState`/`Process`/`ProcessComplex`), строки `state | char | cmd | next` (write — пятёрка); defaults как `FormExport4th` (`,`, `<`, `>`, `#`). **Ограничение V1:** `leftWord`/`rightWord`/`copy` без развёртки `CreateComplexMachine` — только identity-переходы на условиях; полный инлайн — как в десктопе позже.
- **Pan и фон** — `computeWorldSurface` расширяет мировую область (фон + сетка) при сдвиге `viewOffset`, чтобы в видимой области не было «пустоты» вне холста.

## A — Approach

- **Доменное ядро отдельно от UI.** Каталог `@core/*` — чистый TypeScript без React/konva. Это:
  - даёт юнит-тесты без DOM;
  - облегчает порт алгоритмов (`Pathfinder`, `DiagramBezier`, `VMTDebuger`, `VMTSerializer`, `VmtExportPlantUml`, `VMTExport4th`) практически 1-в-1 с C++ — с заменой `QPoint/QRect` на простые value-объекты.
- **Иммутабельная модель.** Любая правка диаграммы возвращает новый `Project`. Undo/Redo — стек ссылок. Это даёт детерминированный диф для перерисовки konva-слоёв.
- **react-konva как сцена.** Разделение слоёв: фон+сетка, рёбра (Path2D из Bézier), узлы, overlay (выделение/анимация шага). Hit-test связей через flatten-полилинию.
- **Две панели в Editor (как десктоп).** Глобальный `AppShell` — навигация и файловые операции (Open/Save/Draft). Экран **Editor** дополнительно содержит: (1) **вертикальную панель инструментов** слева от холста (`tool_frame`, цвет `#01547a`) — выбор режима и типа узла; (2) **горизонтальную панель действий редактора** над холстом — Undo, Export PNG/PUML, Run (→ Compiler), 4th, Zoom. Состав и активность кнопок соответствуют `UIStateEditorMachine` (см. `forms-and-scenarios.md` §2.2).
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
│   │   ├── widgets/
│   │   │   ├── EditorToolbar.tsx      # вертикальная tool_frame (pointer, узлы, link, pan, delete)
│   │   │   ├── EditorActionBar.tsx    # Run / Export / Undo / Zoom (состояние Editor)
│   │   │   └── …                      # alphabet, tape, conditions
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
16. Реализовать экраны Main/Compiler/Debugger/Exercises/Export4th.
16c. **Отладчик — навигация по ленте:** `tapeViewStart` + кнопки `« ‹ ⌂ › »`; Reset сбрасывает окно; тест `tape-viewport.test.ts`.
16d. **Submachine:** `SubmachinePanel`, `BodyBreadcrumb`, drill-down; тесты `debugger-complex`, `submachine-factory`, `project-json-complex`, `jdtp-complex`, `compiler-checks-complex`.
16a. **Editor:** вертикальная `EditorToolbar` + `EditorActionBar` + холст в одном layout (`editor-layout`). Smoke: на `/editor` видны обе панели; переключение инструмента подсвечивается; Run ведёт на `/compiler`; Export PUML скачивает файл.
16b. **Инструменты на холсте:** pointer (выделение; drag узла — прямоугольник+подпись и связанные связи в live preview; drag подписи связи; drag по фону → pan); link; pan; delete; узлы по клику на сетке; `routeAllTransitions` при drag и после commit; inspector-оверлей. Smoke: при drag узла видны подпись внутри квадрата и движущиеся входящие/исходящие линии.
17. Реализовать экспорты: PNG → `OffscreenCanvas` + download; PlantUML; **четвёрки** — `@core/export/fourth.ts` + `Export4thScreen` (экспорт и копирование в буфер). Smoke: простая цепочка start→right→finish даёт строки с `>`; рекурсивный complex — ошибка.
18. Реализовать Undo/Redo через стек снапшотов `Project`.
19. Локализация (i18next, RU/EN), переключатель в шапке.
19a. **Тема:** светлая / тёмная (`ThemeProvider`, `data-theme` на `<html>`, CSS-переменные в `index.css`, палитра канваса в `useTheme().palette`); переключатель **Dark/Light** в toolbar; выбор в `localStorage` (`vmt:theme`).
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
