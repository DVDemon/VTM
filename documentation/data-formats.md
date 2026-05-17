# 3. Форматы сохраняемых и читаемых данных

[← Назад к оглавлению](index.md)

## 3.1. Файл проекта `.vmt.json` (JSON)

Текстовый формат, совместимый с **vmt-web** (`vmt-web/src/core/serializer/json.ts`). Реализация в десктопе: `VMTJsonSerializer`.

Структура корня:

- `schemaVersion` (1)
- `id`, `name`, `alphabet`
- `bodies[]` — массив `MachineBody` (корневая и вложенные подмашины)
- `rootBodyIds[]` — id тел верхнего уровня (в десктопе — список машин проекта)
- `createdAt`, `updatedAt` (ISO 8601)

Каждое `MachineBody`: `id`, `name`, `alphabet`, `machines[]`, `transitions[]`. Узел `complex` содержит `name` и `innerId` (ссылка на другое тело). Условия перехода: `conditions.enabledSigns[]`.

**Открытие / сохранение:** диалог Open — фильтр `*.vmt.json`, `*.json`; в настройках проекта — **Save as JSON…**; бинарный `.jdtp` по-прежнему через кнопку Save.

## 3.2. Файл проекта `.jdtp`

Файл — бинарный поток `QDataStream` (порядок байт — Qt-дефолтный, big-endian). Реализация: `VMTSerializer` (`VMTSerializer.cpp`).

### Общая структура

```
[alphabet : string]
[machine_count : qint64]
repeat machine_count раз:
    [VMTComplexMachine top-level]
```

### Строки

```
[length : qint16]                     // длина в байтах UTF-8
[bytes  : qint8 × length]             // полезная нагрузка
```

(см. `VMTSerializer::WriteString`/`ReadString`).

### Комплексная машина (верхний уровень)

```
[stub geometry]                       // VMTComplexMachine::Serialize (bounds, center, size, power)
[name           : string]
[alphabet       : string]
[inner_marker]                        // bool (true=inner, false=ссылка) + qint64 inner_id
[total_components : qint64]
repeat total_components раз:
    [type_id : qint32]                // 1 = MACHINE, 2 = TRANSITION
    ...
```

### Простая (атомарная) машина

```
[type_id = 1]
[id           : qint64]               // уникальный ID в файле
[machine_type : qint32]               // IVMTMachine::MachineType
[machine body]                        // см. ниже «Узлы»
```

### Вложенная композитная машина

```
[type_id = 1]
[id           : qint64]
[machine_type = 9]                    // MT_COMPLEX
[is_inline    : bool]                 // true=inline definition, false=reference
[inner_id     : qint64]
[VMTComplexMachine ... ]              // рекурсивно
```

Поле `inner_id` обеспечивает разделяемое тело: одну и ту же машину можно использовать многократно — внутри сериализуется один раз, остальные ссылки восстанавливаются `ReplaceInnerObject()`.

### Переход

```
[type_id = 2]
[id             : qint64]
[start_id       : qint64]             // ссылка на машину; 0 = висячий конец
[finish_id      : qint64]
[transition body]                     // VMTTransitionImpl::Serialize: координаты ломаной, условия,
                                      // _conditions_point, флаги
```

При десериализации `DeserializeTransition()` восстанавливает связи `weak_ptr` по ID и добавляет переход в `incoming/outgoing` обеих сторон.

### Узлы (геометрия)

Формат **v2 (текущий)** в `VMTMachineStub::Serialize`:

```
marker = -1   : qint32   // дискриминатор v2
center        : QPoint
size          : QPoint
power         : qint64
```

Формат **v1 (legacy)** — если первый int не равен `-1`, читается как левый край старого `QRect bounds`, далее: `top, right, bottom`, затем `size` и `power`. Размер и позиция восстанавливаются без вызова `Update()`, чтобы не сдвигать координаты.

### Условия перехода

`VMTTransitionConditions::Serialize/Deserialize` сохраняет строку включённых символов алфавита плюс ссылку на алфавит-источник.

## 3.2. Упражнения `exercises.xml`

Встроенный ресурс (`:/config/exercises.xml`), читается через `QXmlStreamReader`.

```xml
<exercises>
  <exercise id="1"
            text_en="..."   text_ru="..."
            completed="0|1" alphabit=" 01">
      <test input="..." output="..."/>
      ...
  </exercise>
  ...
</exercises>
```

- `alphabit` — строка символов алфавита (первый символ — λ/пробел).
- `completed` — флаг «упражнение решено» (сохраняется через `Configuration`).
- `test` — пары вход/ожидаемый выход для автоматической проверки в отладчике.

## 3.3. Конфигурация приложения

Класс `Configuration` (singleton). Файл `config.xml` рядом с приложением: список недавних проектов (`<project>`), флаг **`<darkTheme>1</darkTheme>`** (тёмная тема, `VmtTheme::setDarkMode`). Переключатель — чекбокс на экране настроек проекта.

## 3.4. Ресурсы

`VirtualTuringMachine/resources.qrc`:

- `:/config/exercises.xml`
- иконки тулбаров (`:/Files/images/toolbars/main/*.png`)
- изображения узлов (`MACHINE_*`, `LINK`, `NAVIGATION_CIRCLE`)
- иконки статусов (`ok_black.png`, `cancel.png`)

## 3.5. Лента (runtime)

`VMTLine` хранит ленту блоками `SHIFT_STEP = 10` символов (`char*`). При выходе позиции головки за границы массив автоматически расширяется в нужную сторону и заполняется λ (пустой символ алфавита). См. [`interpreter.md`](interpreter.md#бесконечная-двусторонняя-лента).
