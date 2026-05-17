# Анализ для перевода VMT на веб-платформу (Specification First + SPDD)

Документ — план первой итерации. Код не меняется; цель — зафиксировать требования, ADR, архитектуру (Structurizr DSL), доменную модель, тесты, риски и открытые вопросы перед генерацией кода.

Исходное задание — текущая реализация на C++/Qt, описанная в:
- [`../index.md`](../index.md) — оглавление;
- [`../overview.md`](../overview.md), [`../forms-and-scenarios.md`](../forms-and-scenarios.md), [`../data-formats.md`](../data-formats.md), [`../export-formats.md`](../export-formats.md), [`../interpreter.md`](../interpreter.md), [`../machines.md`](../machines.md), [`../rendering.md`](../rendering.md).

## 0. Выбранный стек (по ответам пользователя)

| Слой | Технология | Обоснование (детали → ADR) |
|---|---|---|
| Frontend | **Vite + React 18 + TypeScript** | минимальный SPA, простой публикуется на GH Pages, типобезопасность для модели машин/переходов. |
| Состояние | **React Context + localStorage** | без внешних зависимостей; ограничение размера выносится в риски. |
| Холст | **react-konva (Canvas 2D)** | подходящая сцена с hit-test и трансформациями, перекрывает потребности рендера узлов/Безье-связей. |
| Аутентификация | **Отсутствует** | приложение полностью клиентское; данные локальны. |
| Scope V1 | **Полный паритет с десктопом** | редактор + интерпретатор + экспорты + упражнения + Undo. |
| Хостинг | **GitHub Pages** | статическая сборка `vite build`, маршрутизация через `HashRouter`. |
| Тесты | Vitest + React Testing Library, Playwright (smoke), node-test для чистого ядра | юнит-тесты независимы от DOM (ядро TS). |

## 1. Артефакты спецификации

Артефакты, которые нужно создать/обновить до генерации кода:

| Артефакт | Путь | Статус |
|---|---|---|
| Анализ и план | `documentation/prompts/analysis.md` | создан (этот файл) |
| REASONS Canvas | `documentation/prompts/reasons-canvas.md` | создать |
| ADR-001 «Frontend SPA: Vite + React + TS» | `documentation/adr/adr-001-frontend-spa-vite-react.md` | создать |
| ADR-002 «Холст: react-konva» | `documentation/adr/adr-002-canvas-react-konva.md` | создать |
| ADR-003 «State + Storage: Context + localStorage» | `documentation/adr/adr-003-state-context-localstorage.md` | создать |
| ADR-004 «Маршрутизация связей: A* + Bézier (порт ядра)» | `documentation/adr/adr-004-routing-astar-bezier.md` | создать |
| ADR-005 «Формат проекта: JSON v1 + обмен `.jdtp` через локальный файловый диалог» | `documentation/adr/adr-005-project-format-json.md` | создать |
| ADR-006 «Без аутентификации в V1; данные — локально» | `documentation/adr/adr-006-no-auth.md` | создать |
| ADR-007 «Деплой на GitHub Pages: HashRouter, base path» | `documentation/adr/adr-007-deploy-github-pages.md` | создать |
| Архитектура C4 | `documentation/workspace.dsl` | создать по шаблону `templates/workspace.dsl` |
| Каталог ADR | `documentation/adr/` | создать |
| Уточнённые требования | внести в Canvas + ниже §2 | создать |

## 2. Требования

Требования делятся на функциональные (FR) и нефункциональные (NFR). Каждое имеет ID и критерий приёмки. «Не в V1» вынесено в §11.

### 2.1. Функциональные требования

| ID | Требование | Источник | Критерий приёмки |
|---|---|---|---|
| FR-001 | Создание/открытие/сохранение проекта | desktop: `VMTSerializer` | Кнопки «New» / «Open» / «Save»: рабочая копия в памяти (React Context); опционально автосохранение черновика как строки `.vmt.json` в localStorage; явное сохранение на диск — файл `.vmt.json` или `.jdtp` (скачивание). Round-trip `.vmt.json` и `.jdtp` через файловый слой покрыт юнит-тестами ядра. |
| FR-002 | Редактирование алфавита | `formalphabit.ui` | UI позволяет добавлять/удалять символы; первый символ интерпретируется как λ; алфавит распространяется на условия переходов. |
| FR-003 | Визуальный редактор диаграммы | `FormEditorMachine` | Можно добавить все типы узлов (`MT_START..MT_COMPLEX`), переместить, удалить, выделить, изменить `power`. |
| FR-004 | Композиция (вложенные машины) | `VMTComplexMachine` | Узел `MT_COMPLEX` открывается двойным кликом; внутри полнофункциональный редактор; глубина не ограничена. |
| FR-005 | Переиспользование машин | `inner_id` в сериализаторе | Одну машину можно вставить в несколько мест без копирования тела; правка в одном месте отражается во всех. |
| FR-006 | Связи (переходы) с условиями | `VMTTransitionImpl`, `VMTTransitionConditions` | Drag от выходного порта; редактор условий показывает символы алфавита; условие «все» сворачивается в `*`. |
| FR-007 | Автомаршрутизация переходов | `pathfinder.cpp` | A* с обходом узлов и уже построенных связей; первый/последний сегмент горизонтальный, входит в центр порта. |
| FR-008 | Кривые Безье на коннекторах | `diagrambezier.cpp` | Линии связи — кубический Безье через узловые точки; стрелка на конце. |
| FR-009 | Проверки корректности диаграммы | `FormCompilerMachine` | Перед запуском прогоняются 5 проверок (`CheckStart/Finish/Connection/AllCases/Dublicate`); ошибки показаны списком. |
| FR-010 | Бесконечная двусторонняя лента | `VMTLine` | Головка может уйти как угодно далеко в любую сторону; пустые ячейки = λ; визуализация прокручивается. |
| FR-011 | Пошаговое исполнение | `VMTDebuger::Step` | Кнопка «Step» делает один такт; текущий узел подсвечен; стек вложенных вызовов корректно работает с FINISH. |
| FR-012 | Непрерывное исполнение | `RunTimer` | Кнопка «Run» запускает цикл с интервалом ~50 мс; «Stop» прерывает; «Pause» приостанавливает. |
| FR-013 | Множитель `power` | `IVMTMachine::GetPower` | Атомарные машины выполняются `power` раз, для COMPLEX состояние повторно кладётся в стек `power-1` раз. |
| FR-014 | Экспорт в PNG | `FormEditorMachine::ExportMachine` | По кнопке «Export PNG» сохраняется изображение, идентичное холсту (фон, сетка, узлы, связи, метки). |
| FR-015 | Экспорт в PlantUML | `VmtExportPlantUml` | По кнопке «Export PUML» формируется `.puml` State Machine: вложенные `state {}`, `[*]` для start/finish, метки `*`/`ε`/строка. |
| FR-016 | Экспорт в «четвёрки» | `VMTExport4th` | Текстовый поток `state | "char" | cmd | next`; настраиваются разделитель/кавычки/обозначения движений; рекурсия отлавливается с понятным сообщением. |
| FR-017 | Режим упражнений | `formexercises`, `exercises.xml` | Список упражнений загружается из встроенного ресурса; запуск автоматически прогоняет все тесты; результат сохраняется в localStorage. |
| FR-018 | Undo/Redo | `VMTUndoManager` | Любая правка диаграммы и алфавита откатывается; глубина стека ≥ 50. |
| FR-019 | Зум холста | `slider_zoom` | 25%…200% (шаг 5%); зум сохраняется в проекте. |
| FR-020 | Открытие `.jdtp` с диска пользователя | `VMTSerializer` v1/v2 | Кнопка «Open .jdtp» → `<input type="file">`; парсер бинарного `QDataStream` в JS (`DataView`); содержимое **не сохраняется в удалённом хранилище** и **не кладётся в localStorage как бинарный blob** — только в оперативную модель после загрузки. Пройден тест на эталонных файлах. |
| FR-023 | Сохранение диаграммы как `.jdtp` на диск | desktop save `.jdtp` | Кнопка «Save .jdtp» формирует `ArrayBuffer` инициирует скачивание файла (`download` или File System Access API); формат байт-в-байт совместим с десктопным экспортом; round-trip тест с эталонным файлом. |
| FR-021 | Локализация (RU/EN) | `exercises.xml: text_en/text_ru` | Переключатель языка в шапке; интерфейс и тексты упражнений локализованы. |
| FR-022 | Сохранение последних проектов | `Configuration` | Меню «Recent» с последними 10 проектами; читается из localStorage. |

### 2.2. Нефункциональные требования

| ID | Требование | Критерий приёмки |
|---|---|---|
| NFR-001 | SPA для GitHub Pages | `vite build` → статика, размещаемая по `https://<user>.github.io/VMT/`; HashRouter; CI публикует на `gh-pages` ветку. |
| NFR-002 | Без бэкенда и серверов | Полностью клиентское: нет сетевых вызовов в рантайме (кроме опционального fetch `exercises.json`). |
| NFR-003 | Без аутентификации | Все данные локальны для браузера; нет хранения PII; нет токенов. |
| NFR-004 | Кросс-браузерность | Chrome / Firefox / Safari последние 2 мажорные версии; mobile Safari как «view-only» допустим. |
| NFR-005 | Доступность | Видимый фокус, ARIA-атрибуты у тулбара и кнопок, отзывчивая палитра (Material Blue из десктопа). |
| NFR-006 | Производительность UI | На сцене 200 узлов + 400 связей: FPS ≥ 30 при drag; перерисовка локальная (dirty-rects через konva layers). |
| NFR-007 | Покрытие тестами | Ядро (модель, интерпретатор, сериализатор, pathfinder, plantuml-экспорт) — ≥ 80% покрытия; smoke E2E через Playwright. |
| NFR-008 | Размер бандла | `<350 KB` gzip начального бандла; ленивая загрузка экранов отладчика и упражнений. |
| NFR-009 | Типобезопасность | strict TS, отсутствие `any` в публичном API ядра. |
| NFR-010 | Воспроизводимость | `package-lock.json` зафиксирован; CI собирает на Node LTS. |
| NFR-011 | Поддержка офлайн | Service Worker (Workbox) кэширует статику; работает без сети после первого визита. (Можно вынести в V1.1, см. §11.) |
| NFR-012 | Лицензия | MIT, как у текущей реализации; третьи-стороны проверены. |
| NFR-013 | Локальный запуск для отладки | `npm install && npm run dev` поднимает Vite dev-сервер (по умолчанию `http://localhost:5173`); в README описаны команды, базовый URL (`vite.config` `server`), переменные окружения при необходимости; разработчик может подключить React DevTools и отладчик браузера без деплоя на GH Pages. |

## 3. Архитектурные решения (ADR-список)

Каждое решение — отдельный ADR по шаблону `templates/adr-template.md`. Содержание (что зафиксировать) — ниже.

| ADR | Решение | Ключевые альтернативы | Влияние |
|-----|---------|----------------------|---------|
| ADR-001 | Vite + React 18 + TypeScript | Next.js (избыточен для SPA), Remix SPA | определяет сборку, deploy, тестирование |
| ADR-002 | react-konva для холста | PixiJS, SVG/D3, React Flow | определяет рендеринг, hit-test, экспорт PNG |
| ADR-003 | React Context + localStorage | Zustand+IDB, Redux Toolkit+IDB | определяет лимиты на размер проекта |
| ADR-004 | A* (порт из C++) + кубические Безье (порт `diagrambezier`) | dagre, elkjs, авто-сплайны | определяет качество маршрутов и тестируемость |
| ADR-005 | JSON v1 как нативный формат + **импорт/экспорт `.jdtp` только через файловый диалог на диске** | хранить `.jdtp` в IndexedDB/localStorage | совместимость с десктопом без серверного хранилища |
| ADR-006 | Без аутентификации в V1 | GitHub OAuth, Supabase | определяет приватность и сложность |
| ADR-007 | GitHub Pages + HashRouter + `base: '/VMT/'` | Netlify/Vercel | определяет CI и URL-схему |

## 4. Архитектура (что положить в `documentation/workspace.dsl`)

По шаблону `templates/workspace.dsl`:

### Persons
- `student` — «Студент» (использует редактор и отладчик);
- `instructor` — «Преподаватель» (составляет упражнения, проверяет решения).

### External systems
- `github_pages` — хостинг статики;
- `browser_storage` — браузерное хранилище (localStorage, при необходимости IndexedDB).

### Software system `vmt_web`
Контейнеры:

| Контейнер | Технология | Назначение |
|---|---|---|
| `spa` | Vite, React 18, TypeScript | Точка входа, маршрутизация, layout |
| `editor` | React + react-konva | Визуальный редактор диаграммы (узлы, связи, drag, zoom) |
| `core` | TypeScript (чистый, без UI) | Доменная модель машин/переходов/алфавита, интерпретатор, сериализатор, pathfinder, plantuml-экспорт |
| `storage` | React Context + localStorage | Хранение проектов, недавних, прогресса упражнений, настроек |
| `exercises_resource` | статический JSON в бандле | База упражнений (мигрирована из `exercises.xml`) |

### Component (C3) для контейнера `core`

Компоненты с `properties { type capability }`:
- `domain_model` (TC-001) — иммутабельные структуры `Machine`, `ComplexMachine`, `Transition`, `Alphabet`, `Tape`.
- `interpreter` (TC-002) — `Debugger`, шаги, стек вложенных вызовов, бесконечная лента.
- `compiler_checks` (TC-003) — 5 проверок диаграммы перед запуском.
- `pathfinder` (TC-004) — A*, упрощение, ортогонализация, выравнивание портов.
- `bezier_renderer` (TC-005) — построение `Path2D` для Безье и flatten для hit-test.
- `serializer_json` (TC-006) — сохранение/загрузка JSON-проекта.
- `serializer_jdtp_io` (TC-007) — кодирование и декодирование бинарных `.jdtp` v1/v2 через `ArrayBuffer` (порт `VMTSerializer`).
- `exporter_png` (TC-008) — рендер сцены в `OffscreenCanvas` → `Blob`.
- `exporter_plantuml` (TC-009) — генератор `.puml`.
- `exporter_4th` (TC-010) — генератор «четвёрок».
- `undo_manager` (TC-011) — стек снапшотов модели.

### Связи
- `student → spa`: «Использует редактор» (`HTTPS`);
- `spa → editor`: «Рендерит сцену»;
- `editor → core`: «Вызывает доменные операции»;
- `core → storage`: «Сохраняет/читает проект» (`send=Project, return=Project`);
- `spa → exercises_resource`: «Загружает упражнения» (`return=Exercise[]`);
- `spa → github_pages`: «Раздаётся как статика» (deployment).

### Deployment view `Production`
- `deploymentNode "GitHub Pages"` → `containerInstance spa`;
- `deploymentNode "Browser"` → `containerInstance storage` (localStorage), `containerInstance editor`, `containerInstance core`.

### Dynamic views (по technical capability)
- `001` — открытие проекта (`spa → storage → core → editor`);
- `002` — пошаговое исполнение (`editor → core.interpreter → editor`);
- `009` — экспорт в PlantUML (`editor → core.exporter_plantuml → browser download`).

## 5. Доменные сущности и связи

| Сущность | Поля | Связи |
|---|---|---|
| `Project` | `id`, `name`, `alphabet`, `machines[]`, `createdAt`, `updatedAt`, `schemaVersion` | имеет много `ComplexMachine` |
| `Alphabet` | `signs: string` (первый = λ) | принадлежит `Project`/`ComplexMachine` |
| `Machine` (абстракт) | `id`, `type: MachineType`, `center`, `size`, `power`, `parentId` | принадлежит `ComplexMachine` |
| `ComplexMachine` extends `Machine` | + `innerId`, ссылается на разделяемое `MachineBody` | содержит `machines[]`, `transitions[]` |
| `MachineBody` | `id`, `machines[]`, `transitions[]`, `alphabet` | разделяется между `ComplexMachine`-обёртками |
| `AlphaMachine` (MT_WRITE) | + `sign: char` | — |
| `Transition` | `id`, `startMachineId`, `finishMachineId`, `routingPoints[]`, `conditions`, `conditionsAnchor` | принадлежит `MachineBody` |
| `TransitionConditions` | `enabledSigns: Set<char>` | принадлежит `Transition` |
| `Tape` (runtime) | блоки по 10 символов, `headIndex`, `zeroIndex` | принадлежит `DebuggerState` |
| `DebuggerState` (runtime) | `currentMachineId`, `currentScopeId`, `callStack: StackFrame[]` | использует `Tape`, `Project` |
| `Exercise` | `id`, `text_en`, `text_ru`, `alphabet`, `tests: {input, output}[]`, `completed` | независимый ресурс |

`MachineType` = `'left' | 'right' | 'leftWord' | 'rightWord' | 'write' | 'copy' | 'start' | 'finish' | 'complex'`.

## 6. Что нужно реализовать

Сгруппировано по этапам — каждый шаг проверяемый.

### Этап A. Каркас проекта (1–2 дня)
1. `vite create` + ESLint + Prettier + Vitest + Playwright.
2. `tsconfig` strict; алиасы `@core`, `@ui`, `@storage`.
3. CI (GitHub Actions): lint → test → build → deploy на `gh-pages`.
4. `HashRouter`, экраны-заглушки: Main, Editor, Debugger, Exercises, Compiler, Export4th.

### Этап B. Доменное ядро `@core` (5–7 дней)
5. Структуры `Project/Machine/Transition` (иммутабельные, `readonly`).
6. `Alphabet` + редактор символов.
7. `Tape` с блоками по 10 символов (порт `VMTLine.cpp`).
8. `Debugger.step()` со стеком (`MT_COMPLEX`, `power`).
9. 5 валидаций (`compiler_checks`).
10. JSON-сериализатор V1.
11. Парсер и генератор `.jdtp` (`encode`/`decode`, DataView); только через файловый слой UI, без сохранения blob в localStorage.
12. Тесты ядра (Vitest) ≥ 80%.

### Этап C. Маршрутизация и Безье (3–4 дня)
13. Порт `Pathfinder` (A* + simplify + ortho + terminal directions).
14. Порт `DiagramBezier` (Catmull–Rom → cubic, flatten, hit-test).
15. Юнит-тесты pathfinder на 10+ сценариях (узлы-препятствия, разнесение параллельных линий).

### Этап D. Холст и редактор (5–7 дней)
16. `react-konva` сцена: фон, сетка, узлы, связи.
17. Drag-n-drop узлов, snap-to-grid, zoom, pan.
18. Создание/удаление связей; preview во время drag.
19. Контекстное меню узла (set power, delete).
20. Редактор условий перехода.

### Этап E. Экраны (4–5 дней)
21. Main: список недавних проектов, Open/New/Import.
22. Editor с тулбаром (как `mainwindow.ui`).
23. Compiler с listом ошибок и переходом к узлу.
24. Debugger с лентой, Step/Run/Stop, режим тестов.
25. Exercises: список из встроенного JSON.

### Этап F. Экспорты (3 дня)
26. PNG через `OffscreenCanvas` + `toBlob` + `URL.createObjectURL`.
27. PlantUML (порт `vmtexportplantuml.cpp`).
28. Четвёрки (порт `vmtexport4th.cpp`); без рекурсии (та же проверка).

### Этап G. Полировка (2 дня)
29. Локализация (RU/EN) через i18next.
30. Undo/Redo через снапшоты `Project`.
31. Service Worker (опционально, V1.1).
32. Accessibility-проход; QA-чеклист.

## 7. Тестовая стратегия

| Уровень | Инструмент | Что покрываем |
|---|---|---|
| Unit (ядро) | Vitest | модель, `Debugger.step()`, `Tape`, валидации, JSON round-trip, `.jdtp` encode/decode, pathfinder, bezier, plantuml/4th экспорты |
| Component | RTL | тулбар, редактор условий, виджет ленты, список упражнений |
| Visual (опц.) | Playwright + screenshots | редактор с 3 фикстурными проектами |
| E2E smoke | Playwright | New project → add nodes → connect → run → check tape |
| Property-based (опц.) | fast-check | свойство «JSON serialize → deserialize = identity» |

### Acceptance-сценарии (Gherkin-style, для CI)
- `feature/open-legacy-jdtp`: открываю файл из `tests/fixtures/*.jdtp` → диаграмма идентична эталонной JSON-сериализации.
- `feature/step-execution`: программа «инкремент двоичного числа» из FR-017 проходит все тесты упражнения 1.
- `feature/export-plantuml`: для эталонной диаграммы `.puml` совпадает байт-в-байт с фикстурой.

## 8. Риски и открытые вопросы

| # | Риск/вопрос | Влияние | Митигация / решение |
|---|---|---|---|
| R-1 | **localStorage ≤ 5 MB и только строки** | большие проекты не помещаются; бинарные `.jdtp` хранить нельзя | хранить как JSON; крупные blob (PNG-preview) — не сохранять; для V1.1 — мигрировать на IndexedDB. Открытый вопрос: лимит «крупного проекта» (см. §10). |
| R-2 | **Парсер бинарного `.jdtp` (QDataStream big-endian, qint64)** | сложность портирования v1/v2 формата на JS | прототип на эталонных файлах в самом начале (Этап B, шаг 11); если не успеем — V1.1 |
| R-3 | **Производительность react-konva на 500+ узлах** | возможный лаг | разделение слоёв (background/edges/nodes/overlay), batchDraw, listening только нужным узлам |
| R-4 | **PNG-экспорт идентичный десктопу** | разные шрифты/толщины | использовать тот же `OffscreenCanvas`, что и для рендера, и заранее зафиксировать font-stack |
| R-5 | **Pathfinder A\* в JS медленнее C++** | задержки при пакетной перестройке | web worker для перестройки маршрутов; кэш |
| R-6 | **Совместимость старого ZUI-десктопного формата на чтение** | пользователи могут потерять данные | покрыть тестами на эталонных файлах в `tests/fixtures` |
| R-7 | **Без аутентификации → нет облачной синхронизации** | ограничивает мульти-устройство | export/import JSON-файла; V1.1 — sync через Gist |
| R-8 | **Безопасность localStorage** | xss → утечка проектов | строгая CSP, dependabot, отсутствие inline-скриптов |

## 9. Открытые вопросы (требуют решения до генерации кода)

1. **Хранилище**: подтвердить, что V1 действительно ограничивается localStorage; нужен ли запасной путь Export-as-JSON для проектов > 4 MB? *(в Canvas — Norms/Safeguards)*
2. **Маршрутизация в worker**: разрешено ли тащить web worker в V1 или это V1.1?
3. **Локализация**: оставлять ли английский по умолчанию или авто-детект `navigator.language`?
4. **Schema-эволюция JSON**: фиксируем ли `schemaVersion: 1` сразу с миграциями или допускаем «breaking» в V1.x?
5. **Файловые операции**: использовать File System Access API (Chromium) с fallback на `<input type="file">`/`download` для Safari/FF?
6. **Service Worker**: входит ли в V1 (NFR-011) или едет в V1.1?
7. **Тесты на эталонных `.jdtp`**: где взять фикстуры (нужно ли их сгенерировать текущим C++ бинарём)?

## 10. Что НЕ входит в V1

- Облачная синхронизация / sharing по ссылке.
- Аутентификация (OAuth, Supabase и т. п.).
- Совместное редактирование (multiplayer).
- Импорт/экспорт PlantUML обратно в модель.
- Мобильный touch-режим редактора (только просмотр).
- Серверная проверка решений упражнений.
- Хранение крупных проектов > 4 MB в браузере (используем экспорт в файл).
- Голосовой/AI-ассистент.

## 11. Секции REASONS Canvas, требующие ручного уточнения

После генерации `reasons-canvas.md` нужно ревью со стороны пользователя по секциям:

- **R (Requirements):** scope «полный паритет» — действительно ли в V1 нужны упражнения и Undo, или вынести в V1.1?
- **E (Entities):** имена/поля доменных типов (особенно `MachineBody` vs `ComplexMachine.inner`).
- **A (Approach):** «порт алгоритмов 1-в-1 vs. реализация на чистом TS с другой структурой» — ожидаемая стратегия.
- **S (Structure):** структура папок и алиасов; должен ли `core` быть отдельным npm-пакетом для последующего переиспользования.
- **O (Operations):** очерёдность этапов A–G — допустимо ли начать с holiday-демки (этап D без полного ядра)?
- **N (Norms):** правила code style, длина PR, обязательность ADR при изменении публичного API ядра.
- **S (Safeguards):** что считать «приватными данными» (имена файлов? содержимое ленты?).

## 12. Ключевые требования и секции Canvas для первой итерации

(см. краткое объяснение в ответе ассистента после применения промпта)
