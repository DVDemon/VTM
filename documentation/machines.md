# 6. Внутреннее устройство машин Тьюринга

[← Назад к оглавлению](index.md)

## 6.1. Иерархия классов

```
IVMTMachine                  (interfaces/IVMTMachine.h)         — абстрактный интерфейс
└── VMTMachineStub           (vmtmachines/VMTMachineStub.h)     — общая логика, геометрия, связи
    ├── VMTMachineStart
    ├── VMTMachineFinish
    ├── VMTMachineLeft
    ├── VMTMachineRight
    ├── VMTMachineLeftWord
    ├── VMTMachineRightWord
    ├── VMTMachineAlpha            — запись произвольного символа (MT_WRITE)
    ├── VMTMachineCopy             — копирование «слова»
    └── VMTComplexMachine          — композитная машина-контейнер
            ▸ VMTComplexMachineInner   — хранит коллекции узлов и переходов
```

```
IVMTTransition  (interfaces/IVMTTransition.h)
└── VMTTransitionImpl     (VMTTransitionImpl.h/.cpp)
        ▸ VMTTransitionConditions   — условия срабатывания (набор символов алфавита)
```

## 6.2. Перечень типов машин

`IVMTMachine::MachineType`:

| ID | Константа | Назначение | Действие на ленту |
|----|-----------|-----------|---|
| 1 | `MT_LEFT` | сдвиг головки на одну ячейку влево | `line.ShiftLeft()` |
| 2 | `MT_LEFT_WORD` | сдвиг влево до ближайшей λ | `line.ShiftLeftWord()`; имеет собственную реализацию через мини-диаграмму |
| 3 | `MT_RIGHT` | сдвиг вправо | `line.ShiftRight()` |
| 4 | `MT_RIGHT_WORD` | сдвиг вправо до λ | `line.ShiftRightWord()` |
| 5 | `MT_WRITE` | запись символа (`VMTMachineAlpha`) | `line.SetSignAtMachine(_sign)` |
| 6 | `MT_COPY` | копирование текущего слова | развёртка во вложенную диаграмму |
| 7 | `MT_START` | стартовая точка комплексной машины | — |
| 8 | `MT_FINISH` | финальная точка | — |
| 9 | `MT_COMPLEX` | вложенная пользовательская машина | вход во внутреннюю диаграмму через стек |

`MT_LEFT_WORD`, `MT_RIGHT_WORD`, `MT_COPY` — «сахар»: они умеют развернуться в эквивалентную диаграмму через `CreateComplexMachine(env)`. Это используется в экспорте «четвёрок» и в отладчике, когда нужно зайти «внутрь».

## 6.3. Базовый узел `VMTMachineStub`

Общие поля:

- `QRect _bounds`, `_bounds_with_childs`, `QPoint _center`, `_size` — геометрия на холсте;
- `QPoint _input_point`, `_output_point` — точки подключения переходов;
- `bool _error`, `_selected` — флаги UI;
- `long _power` — множитель выполнения (повтор N раз);
- `std::weak_ptr<VMTComplexMachine> _parent` — родительская комплексная машина;
- `std::vector<std::weak_ptr<IVMTTransition>> _incoming, _outgoing` — связи.

Общие операции: `Move`, `Paint`, `Serialize`/`Deserialize`, `Update` (пересчёт `bounds_with_childs`), `Add/RemoveIncoming/OutgoingTransition`, `Clone`.

## 6.4. Композитная машина `VMTComplexMachine`

Содержит указатель на разделяемое тело `VMTComplexMachineInner`:

- `std::vector<std::shared_ptr<IVMTMachine>> machines`;
- `std::vector<std::shared_ptr<IVMTTransition>> transitions`;
- собственный `VMTAlphabit`;
- pathfinder-кеш.

Ключевые методы:

- `AddMachine`, `AddTransition`, `RemoveMachine`, `RemoveTransition`;
- `ConnectMachines(first, second, env, parent)` — создание перехода между двумя узлами с автомаршрутизацией;
- `FindMachine`/`FindTransition` — поиск под курсором;
- `GetBoundsWithChilds()` — bounding box всей диаграммы (используется при экспорте PNG);
- `beginRoutingPass`/`endRoutingPass`/`blockedPathsFor`/`recalculateRoutingForMachine` — пакетный пересчёт маршрутов переходов после изменений (для устранения наложений);
- `Paint`/`PaintDiagram` — отрисовка содержимого.

### Разделяемое тело и переиспользование

Один и тот же `VMTComplexMachineInner` может принадлежать нескольким комплексным машинам с разными именами. В сериализаторе это выражается через `inner_id`: первое появление пишет полное тело, остальные — только ссылку. При десериализации `ReplaceInnerObject` восстанавливает разделяемость.

## 6.5. Вложенность и рекурсия

- **Вложенность** — произвольной глубины. Каждая `VMTComplexMachine` имеет своё `VMTComplexMachineInner` с собственными узлами и переходами; внутри могут быть другие `VMTComplexMachine`.
- **Рекурсия** на уровне модели **допустима** (можно поместить машину `A` внутрь себя через ссылку), однако:
  - **интерпретатор** просто заходит в неё через стек (`_stack`) и работает корректно при наличии условия выхода;
  - **экспорт в четвёрки** не поддерживает рекурсию — `VMTExport4th` обходит дерево с проверкой имени в стеке и при обнаружении цикла прерывает экспорт с сообщением `"Recursive machine call to %1 detected in %2"`.
- **Множитель `power`** — у любой машины (атомарной или композитной) можно задать число повторов выполнения. В отладчике повторы реализуются стеком, в экспорте — нумерацией промежуточных состояний.

## 6.6. Переходы (`VMTTransitionImpl`)

Поля:

- `std::weak_ptr<IVMTMachine> _start_machine, _finish_machine` — концы;
- `std::vector<QPoint> _points` — текущая ломаная (preview во время drag);
- `std::vector<QPoint> _committedPoints` — последний зафиксированный маршрут (для пакетной перестройки);
- `QPoint _start_point`, `_finish_point` — точки подключения у портов;
- `VMTTransitionConditions _conditions` — условия (символы алфавита);
- `QPoint _conditions_point`, `double _conditions_segment`, `int _conditions_segment_number` — положение подписи условий.

Маршрут строится `Pathfinder` (см. [`rendering.md`](rendering.md#62-pathfinder-маршрутизация)) поверх сетки, с учётом препятствий — других узлов и уже построенных переходов. Финализируется в `finalizeTransitionPoints`: первый и последний сегмент выравниваются по высоте порта (`DiagramBezier::snapPortHeights`), чтобы стрелка входила в центр узла строго горизонтально.

## 6.7. Условия перехода `VMTTransitionConditions`

- хранит ссылку на `VMTAlphabit` и строку включённых символов;
- API: `SetEnable(char, bool, env)`, `IsEnabled(char)`, `IsAllEnabled`, `IsAllDisabled`, `GetAsString`;
- сериализуется как строка плюс флаг включённости каждого символа алфавита;
- при изменении алфавита (`ChangeAlphabit`) условия пересчитываются.

## 6.8. Алфавит `VMTAlphabit`

Конечное множество символов. Первый символ всегда играет роль **λ (пустого)**: `GetLambda()` возвращает `_signs[0]`. При построении ленты пустые ячейки инициализируются λ. Алфавит хранится на проекте (`VMTProject::_alphabit`) и индивидуально у каждой `VMTComplexMachine` (можно изменять для составных машин, но проект синхронизирует их в `VMTSerializer::FinalizeProject`).
