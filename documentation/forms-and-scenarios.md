# 2. Формы и пользовательские сценарии

[← Назад к оглавлению](index.md)

## 2.1. Состояния UI

UI построен по схеме *State Machine*: главное окно (`MainWindow`, реализует `StateController`) переключает встроенные формы и состав активных кнопок панели инструментов через классы-наследники `UIState`.

| Состояние (`uistate*.h`) | Активная форма | Назначение |
|---|---|---|
| `UIStateMain` | `FormMain` | Стартовый экран: список недавних проектов, доступ к упражнениям. |
| `UIStateNewProject` | `FormNewProject` | Создание нового проекта (имя, расположение, алфавит). |
| `UIStateNewMachine` | `FormNewMachine` | Создание новой машины внутри проекта. |
| `UIStateExercises` | `FormExercises` | Выбор учебного упражнения. |
| `UIStateEditorMachine` | `FormEditorMachine` | Визуальный редактор диаграммы машины. |
| `UIStateDebugerMachine` | `FormDebugerMachine` | Отладчик: пошаговое/непрерывное исполнение, тесты. |
| `UIStateCompilerMachine` | `FormCompilerMachine` | Проверка корректности диаграммы перед запуском. |
| `UIStateExport4th` | `FormExport4th` | Экспорт в «четвёрки». |

Дополнительные модальные/вспомогательные формы:

| Форма (`.ui`) | Назначение |
|---|---|
| `formwarning` | Подтверждение деструктивных действий. |
| `formalphabit` | Редактирование набора символов алфавита. |
| `formcalculator` | Задание множителя (power) исполнения машины. |
| `formcomplexmachines` | Список существующих машин для вставки на диаграмму. |
| `formactionhint` | Подсказка-плашка во время выбора действия. |
| `formlinewidget` | Виджет визуализации ленты в отладчике. |
| `formeditorwidget` | Холст-виджет диаграммы (используется в редакторе/отладчике). |

## 2.2. Главное меню — панель кнопок

Кнопки `mainwindow.ui`:

`button_new_machine`, `button_open_machine`, `button_save`, **`button_export`** (PNG), **`button_export_plantuml`** (PUML), `button_run`, `button_configure`, `button_exercise`, `button_4th` (экспорт в четвёрки), `button_debug_*` (back/run/pause/stop/over/step), `button_ok`/`button_cancel`, `slider_zoom`.

Активность каждой кнопки управляется текущим `UIState` через методы `StateController::Enable*`. Например, кнопка экспорта в PlantUML включена только в `UIStateEditorMachine`.

## 2.3. Основные пользовательские сценарии

### UC-1. Создание проекта
1. `FormMain` → «Новый проект» → переход в `UIStateNewProject`.
2. Пользователь вводит имя, путь, алфавит.
3. По OK сохраняется `.jdtp`, создаётся пустая машина верхнего уровня, переход в `UIStateEditorMachine`.

### UC-2. Открытие проекта
1. `FormMain` → «Открыть» → `QFileDialog` (фильтр `*.jdtp`).
2. `VMTSerializer::Deserialize()` восстанавливает алфавит, машины, переходы.
3. Открывается редактор корневой машины.

### UC-3. Редактирование диаграммы (`UIStateEditorMachine`)
- Выбор инструмента (`vmtactions/*`): указатель, создание узла, связи, удаление, перемещение.
- Добавление узлов разных типов: Start, Finish, Left/Right, LeftWord/RightWord, Write (alpha), Copy, Complex.
- Создание связи: drag от выходного порта одной машины к входному порту другой; маршрут строится автоматически (см. [`rendering.md`](rendering.md)).
- Редактирование условий перехода: выбор символов алфавита, при которых срабатывает связь.
- Сохранение (`button_save`), экспорт в PNG/PUML/четвёрки.

### UC-4. Проверка корректности
1. Кнопка «Run» → `UIStateCompilerMachine`.
2. `FormCompilerMachine` рекурсивно обходит все машины и выполняет проверки:
   - `CheckStart` — ровно один MT_START в каждой комплексной машине;
   - `CheckFinish` — хотя бы один MT_FINISH;
   - `CheckConnection` — у не-Start есть входящая, у не-Finish — исходящая связь;
   - `CheckAllCases` — для каждого узла исходящие переходы покрывают весь алфавит;
   - `CheckDublicate` — нет символов, обрабатываемых сразу двумя исходящими переходами.
3. При ошибках — список с переходом к месту проблемы; при успехе — переход в отладчик.

### UC-5. Отладка / исполнение (`UIStateDebugerMachine`)
- Кнопка `button_debug_step` — один шаг (`VMTDebuger::Step`).
- Кнопка `button_debug_run` — непрерывный режим через `QTimer::singleShot(50, …)` ([`interpreter.md`](interpreter.md#непрерывный-режим)).
- Кнопка `button_debug_stop` — остановка.
- Виджет ленты (`formlinewidget`) показывает символы и положение головки, кнопки `button_left_page` / `button_left` / `button_right` / `button_right_page` — навигация.

### UC-6. Режим упражнений
1. `FormMain` → «Упражнения» → `FormExercises` (список из `config/exercises.xml`).
2. После выбора задачи проект автоматически создаётся с нужным алфавитом, тестами `input → output`.
3. В отладчике автоматически прогоняются все тесты, итог отмечается зелёной/красной иконкой.

### UC-7. Экспорт
- **PNG:** `button_export` → `FormEditorMachine::ExportMachine()` — рендер диаграммы в `QImage` через `UICanvas` и сохранение `QImageWriter`.
- **PlantUML:** `button_export_plantuml` → `FormEditorMachine::ExportPlantUml()` → `VmtExportPlantUml::exportStateMachine()`.
- **Четвёрки:** `button_4th` → `UIStateExport4th` / `FormExport4th` → `VMTExport4th::Export()` (фоновый поток, см. [`export-formats.md`](export-formats.md#четвёрки)).
