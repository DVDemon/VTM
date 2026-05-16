# VTM (Virtual Turing Machine)

Эмулятор машин Тьюринга с визуальным редактором диаграмм.

## Возможности

- Редактор диаграмм для программ машин Тьюринга
- Вложенные диаграммы и рекурсивные вызовы машин
- Сохранение и загрузка проектов (`.jdtp`)
- Экспорт в нотацию «четвёрок»
- Экспорт диаграмм в PNG
- Сборка под Windows, macOS, Linux, Android, iOS

![example](docs/example.png)

## Зависимости

| Компонент | Версия |
|-----------|--------|
| C++ | 17 |
| Qt | 5.15+ (рекомендуется) или 6.5+ |
| CMake | 3.16+ (для сборки через CMake) |
| Компилятор | Clang / GCC / MSVC с поддержкой C++17 |

На **macOS (Apple Silicon)** kit Qt 5.15 `clang_64` собирает бинарник **x86_64** (через Rosetta). Нативная **arm64** сборка — через Qt 6.5+.

## Структура проекта

```
VTM/
├── CMakeLists.txt              # Корневой CMake-проект (VTM + тесты)
├── README.md
├── docs/                       # Документация и скриншоты
├── tests/                      # Unit-тесты (Qt Test)
│   ├── CMakeLists.txt
│   └── serialization/
│       └── test_machine_serialization.cpp
└── VirtualTuringMachine/       # Исходный код приложения
    ├── VirtualTuringMachine.pro # Сборка через qmake / Qt Creator
    ├── CMakeLists.txt          # Библиотека vtm_core + исполняемый файл VTM
    ├── main.cpp                # Точка входа
    ├── mainwindow.*            # Главное окно
    ├── configuration.*         # Настройки и недавние проекты
    ├── form*.cpp/.h/.ui        # Экраны UI (редактор, отладчик, …)
    ├── uistate*.h              # Состояния интерфейса (state machine UI)
    ├── vmtproject.*            # Модель проекта
    ├── VMTSerializer.*         # Сериализация/десериализация проекта
    ├── resources.qrc           # Ресурсы (иконки, конфиги)
    ├── config/
    │   └── exercises.xml       # Упражнения (встроенный ресурс)
    ├── interfaces/             # Абстрактные интерфейсы ядра
    ├── vmtmachines/            # Типы машин Тьюринга
    ├── vmtactions/             # Действия на диаграмме (создание, связи, …)
    ├── vmtexport4th/           # Экспорт в «четвёрки»
    ├── android-sources/        # Манифест для Android-сборки
    └── Resources/              # Иконка приложения (macOS)
```

## Сборка

### Qt Creator (рекомендуется)

1. Откройте `VirtualTuringMachine/VirtualTuringMachine.pro`.
2. Выберите kit:
   - **Qt 6.5+ for macOS** — нативная arm64 на Apple Silicon;
   - **Qt 5.15.2 clang_64** — классическая сборка (x86_64 на Mac).
3. **Сборка → Собрать** (⌘B).

Каталоги сборки Qt Creator (`build-VirtualTuringMachine-*`) в git не попадают.

### qmake (командная строка)

**Qt 6 (macOS, arm64):**

```bash
export PATH="$HOME/Qt/6.5.2/macos/bin:$PATH"
cd VirtualTuringMachine
qmake VirtualTuringMachine.pro CONFIG+=release
make -j$(sysctl -n hw.ncpu)
```

**Qt 5.15 (macOS):**

```bash
export PATH="$HOME/Qt/5.15.2/clang_64/bin:$PATH"
cd VirtualTuringMachine
qmake VirtualTuringMachine.pro CONFIG+=release
make -j$(sysctl -n hw.ncpu)
```

Результат: `VirtualTuringMachine/VirtualTuringMachine.app`.

### CMake

```bash
# Qt 5.15 (путь подставьте свой)
export QTDIR="$HOME/Qt/5.15.2/clang_64"

cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(sysctl -n hw.ncpu)
```

Результат: `build/VirtualTuringMachine/VTM.app` (цель `VTM`, ядро — статическая библиотека `vtm_core`).

На Apple Silicon с Qt 5.15 CMake автоматически выставляет `CMAKE_OSX_ARCHITECTURES=x86_64`.

Опции CMake:

| Опция | По умолчанию | Описание |
|-------|--------------|----------|
| `BUILD_TESTS` | `ON` | Собирать unit-тесты в `tests/` |

Отключить тесты:

```bash
cmake -B build -DBUILD_TESTS=OFF
```

## Тесты

Тесты используют **Qt Test** и библиотеку `vtm_core`. Проверяют round-trip сериализации диаграммы (создание узлов → сохранение → загрузка → сравнение).

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON
cmake --build build --target test_machine_serialization

# Запуск напрямую (headless на macOS)
QT_QPA_PLATFORM=offscreen ./build/tests/test_machine_serialization

# Или через CTest
cd build && ctest -R test_machine_serialization --output-on-failure
```

## Формат проекта (`.jdtp`)

Файл проекта — бинарный поток `QDataStream`:

1. Алфавит (строка UTF-8)
2. Число машин верхнего уровня
3. Для каждой машины — дерево узлов и переходов

**Узлы (формат v2, текущий):** маркер `-1`, затем `center`, `size`, `power`. Координаты сохраняются в каноническом виде, без пересчёта через `Update()` при загрузке.

**Узлы (формат v1, legacy):** `bounds` + `size` + `power`. Старые проекты по-прежнему открываются; границы восстанавливаются из файла без сдвига.

**Переходы:** геометрия и условия читаются до привязки к узлам; связи восстанавливаются без вызова pathfinder на этапе десериализации.

## Запуск

### macOS

После сборки через **qmake**:

```bash
open VirtualTuringMachine/VirtualTuringMachine.app
```

После сборки через **CMake**:

```bash
open build/VirtualTuringMachine/VTM.app
```

Или из Qt Creator: **Запуск** (⌘R).

### Linux / Windows

Запустите собранный исполняемый файл из каталога сборки (`.exe` / бинарник без `.app`).

## Переменные окружения

| Переменная | Назначение |
|------------|------------|
| `QTDIR` | Путь к установке Qt (для CMake и qmake) |
| `PATH` | Добавьте `$QTDIR/bin` для вызова `qmake` из терминала |
| `QT_QPA_PLATFORM` | `offscreen` — для запуска тестов без GUI (macOS/Linux) |

Пример для Qt 5.15 на macOS:

```bash
export QTDIR="$HOME/Qt/5.15.2/clang_64"
export PATH="$QTDIR/bin:$PATH"
```
