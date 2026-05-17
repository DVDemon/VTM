# 7. Принцип отрисовки диаграмм и связей

[← Назад к оглавлению](index.md)

## 7.1. Слой рендеринга `UICanvas`

`UICanvas` (`uicanvas.h/.cpp`) — обёртка над `QPainter`, инкапсулирующая:

- **состояние камеры** (`UICanvasState`): центр сцены и зум (%, 25…200);
- **палитру** (через `VmtTheme`): фон диаграммы, цвет сетки, базовая линия, выделение, ошибка;
- **сетку** (`SetCellSize`, `GetStep`) — модельные координаты дискретны с шагом ячейки;
- **изображения узлов** — массив `QPixmap*` по `ImageType` (`MACHINE_START`, `MACHINE_LEFT_WORD`, …, `MACHINE_COMPLEX`);
- **преобразования координат**: `ToScreen`, `FromScreen` с учётом зума.

Базовые операции рисования:

```cpp
DrawRect, DrawButton, DrawImage, DrawImageScreen
DrawLine, DrawArrow                       // прямые отрезки
DrawConnectorPath(QPainterPath, …)        // криволинейный коннектор (Bézier)
DrawArrowForPath(QPainterPath, …)         // стрелка в конце QPainterPath
DrawRectAnimation, DrawLineAnimation      // мигающая анимация для текущего шага отладчика
DrawText
```

Холст рисует фон + сетку в `PaintBackground`, остальное передаётся объектам диаграммы.

## 7.2. Виджет холста

`FormEditorWidget` — QWidget, который:

1. в `paintEvent` создаёт `QPainter`, прокидывает его в `UICanvas`;
2. вызывает `VMTComplexMachine::PaintDiagram(canvas, rect)`;
3. ловит мышь/клавиатуру и делегирует текущему действию (`vmtactions/*`).

`FormEditorMachine` и `FormDebugerMachine` встраивают этот виджет (`ui->widget_editor`) и реализуют `IVMTEnvironment` (`GetGraphics`, `GetMachine`, `Repaint`, `Move`, `SetMachine`).

## 7.3. Отрисовка узла

Каждый `VMTMachineStub::Paint`:

1. рисует контур (`canvas.DrawRect(_internal_bounds, _selected, _error)`);
2. накладывает соответствующий `QPixmap` через `canvas.DrawImage(ImageType::..., _internal_bounds)`;
3. при необходимости — текст (`VMTMachineAlpha` рисует символ, `VMTComplexMachine` — имя и значок «папки»).

Подсветка выбранного узла достигается через цвет `_pen_selected`. В отладчике текущий узел дополнительно мигает: `UICanvas::Animate()` смещает индекс пера, `DrawRectAnimation`/`DrawLineAnimation` выбирают пера из массива `_animation_pen[10]`.

## 7.4. Отрисовка связи

`VMTTransitionImpl::Paint(canvas, rect)`:

1. строит `QPainterPath` через `DiagramBezier::buildConnectorPath(_points)`:
   - алгоритм — Catmull–Rom через узловые точки, преобразованный в кубические Безье;
   - первый/последний сегменты делаются горизонтальными (касательная вдоль линии порта);
2. рисует кривую — `canvas.DrawConnectorPath(path, selected, error, animated)`;
3. на конце — стрелка `canvas.DrawArrowForPath(path, selected, error)`;
4. отдельно рисует метку условий (`GetAsString()`) около вычисленной точки `_conditions_point` на сегменте `_conditions_segment_number`.

Hit-test (наведение / выбор связи) выполняется в `DiagramBezier::isPointNearStroke` через `QPainterPathStroker` с толерантностью.

## 7.5. Маршрутизация — `Pathfinder`

Класс `Pathfinder` (`pathfinder.h/.cpp`).

### Входные данные
- `start`, `finish` — точки портов;
- `bounds` — общий прямоугольник сцены;
- `grid_size` — размер ячейки маршрутной сетки;
- `wall_checker_t checker` — функция «эта точка занята узлом?»;
- `blockedPaths` — список уже построенных переходов (для разнесения параллельных связей);
- `pathCorridor` — ширина «коридора» вокруг существующих путей.

### Алгоритм
1. **Дискретизация.** Сцена разбивается на сетку `cells_count_x × cells_count_y` ячеек. Каждая помечается стеной либо проходом (`FillWalls`); `markBlockedPaths` помечает коридоры вокруг существующих переходов.
2. **A\*** с восьмисвязной графовой эвристикой и **штрафом за поворот** (`searchPath`) — это сокращает число «зигзагов».
3. **Упрощение** (`simplifyPath`): убираются коллинеарные точки.
4. **Ортогонализация** (`optimizeOrthogonalPath`): нелинейные участки выпрямляются в горизонтально/вертикальные сегменты, если есть проход без столкновений.
5. **Финализация терминалов** (`enforceTerminalDirections`): первый и последний сегмент гарантированно выходят перпендикулярно к границе порта.

### Финальная подгонка под порт
После A* в `VMTTransitionImpl::finalizeTransitionPoints` вызывается `DiagramBezier::snapPortHeights(points)` — крайние «огрызки» выравниваются по высоте центра портов, чтобы стрелка приходила точно в центр узла, без вертикального смещения у самого порта.

### Пакетная перестройка
При движении узла все его переходы становятся «грязными». `VMTComplexMachine::beginRoutingPass()` фиксирует снапшот `_committedPoints`; `recalculateRoutingForMachine` перестраивает маршруты с учётом обновлённых препятствий; `endRoutingPass()` закрывает пакет. `IVMTTransition::GetCommittedRoutingPolyline()` отдаёт «вчерашние» точки, что не мешает перестройке во время drag-preview.

## 7.6. Безье-кривые

`DiagramBezier` (`diagrambezier.h/.cpp`):

```cpp
QPainterPath buildConnectorPath(const std::vector<QPoint>& points);
void         snapPortHeights(std::vector<QPoint>& points);
std::vector<QPoint> flattenPath(const QPainterPath&, qreal maxSegmentLength = 8.0);
bool         isPointNearStroke(const QPainterPath&, const QPoint&, qreal tolerance);
```

- `buildConnectorPath` — Catmull–Rom → кубический Безье; на концах задаёт горизонтальные касательные, чтобы стрелка «выходила» из узла без излома.
- `flattenPath` — превращает кривую в ломаную для тестов и проверок пересечений.
- `isPointNearStroke` — точное попадание клика на линию связи (используется в `IsInside`).

## 7.7. Перерисовка областей и dirty-rects

Перерисовка инициируется `IVMTEnvironment::Repaint(QRect)`. Для устранения артефактов при перетаскивании:

- `VMTActionPointer` хранит `_drag_dirty_rect`, объединяющий старые и новые позиции узла и затронутых связей;
- `IVMTTransition::transitionRepaintBounds` возвращает bbox Безье-пути с увеличенным запасом (`margin`) и учитывает прямоугольник подписи условий;
- `FormEditorWidget::Repaint` расширяет переданный rect ещё на несколько пикселей в screen-coord.

Таким образом, при движении узла перерисовываются только затронутые области, без полной перестройки сцены, и не остаётся «хвостов» от старого положения связи.

## 7.8. Тема `VmtTheme`

`VmtTheme` (`vmttheme.h/.cpp`) — централизованный источник цветов и стилей:

- `diagramBackground`, `diagramGrid`, `diagramLine`, `diagramSelected`, `diagramError` — палитра холста;
- `applicationStyleSheet()` — глобальная QSS-тема (Material Blue) для всего приложения;
- `applyIconToolBar()`, `iconToolButtonStyle()`, `alphabetButtonStyle()` — стили панелей кнопок;
- `polishWidgetTree(widget)` — снимает с дочерних виджетов унаследованные из `.ui` устаревшие цвета.

PNG-экспорт использует ту же палитру — диаграмма получается визуально идентичной редактору.
