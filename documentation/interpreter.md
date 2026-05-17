# 5. Принцип работы интерпретатора

[← Назад к оглавлению](index.md)

Интерпретатор реализован в классе `VMTDebuger` (`vmtdebuger.h/.cpp`) и работает поверх ленты `VMTLine` и дерева машин (`VMTComplexMachine` + наследники `IVMTMachine`). Управляется из формы `FormDebugerMachine`.

## 5.1. Состояние интерпретатора

```cpp
struct State {
    std::shared_ptr<VMTComplexMachine> complex_machine;   // активная область
    std::shared_ptr<IVMTMachine>       machine;           // текущий узел
};

std::stack<State> _stack;        // стек вызовов вложенных машин
std::shared_ptr<VMTLine> _line;  // лента
```

При инициализации `complex_machine` = корневая машина проекта, `machine` — узел `MT_START` внутри неё.

## 5.2. Бесконечная двусторонняя лента

Класс `VMTLine`:

- лента хранится как `std::vector<char*>`, каждый блок длиной `SHIFT_STEP = 10` символов;
- координата `_zero` — индекс первой ячейки в массиве;
- позиция головки `_machine` — абсолютный индекс (может быть отрицательным);
- метод `CheckPosition(index)` при выходе за границы массива:
  - **влево** — вставляет новые блоки в начало, заполняет их λ (`_alphabit->GetLambda()`), сдвигает `_zero` влево;
  - **вправо** — добавляет блоки в конец, заполняет λ.

Так достигается **бесконечная двусторонняя лента**: пользователь может смещаться сколь угодно далеко в любую сторону, потребление памяти растёт линейно по «глубине» использования.

Операции `ShiftLeft`, `ShiftLeftWord`, `ShiftRight`, `ShiftRightWord`, `SetValueAt`, `SetSignAtMachine`, `Clear`, `ToString` инкапсулированы в `VMTLine`.

## 5.3. Шаг интерпретатора (`VMTDebuger::Step`)

В зависимости от типа текущей машины:

| Тип | Действие |
|---|---|
| `MT_START` | переход к следующему узлу по `FindNextMachine` (выбирается переход, чьи `Conditions::IsEnabled(sign)` для символа под головкой = true). |
| `MT_LEFT`, `MT_RIGHT`, `MT_LEFT_WORD`, `MT_RIGHT_WORD`, `MT_WRITE`, `MT_COPY` (атомарные) | `power` раз вызывается `machine->DoAction(line)` (сдвиг ленты или запись символа), затем `FindNextMachine`. |
| `MT_COMPLEX` (вложенная машина) | текущее состояние помещается в стек `_stack` `power` раз, `complex_machine` заменяется на внутреннюю, `machine` — на её `MT_START`. |
| Составные «функции» (`MT_LEFT_WORD`, `MT_RIGHT_WORD`, `MT_COPY`) при наличии собственной развёртки | через `CreateComplexMachine(env)` создаётся диаграмма-эквивалент, заходим в неё через стек. |
| `MT_FINISH` | если стек пуст — выход на корневой `MT_START` (готов следующий прогон); иначе — `pop` стека и `FindNextMachine` от вернувшегося узла. |

После каждого шага текущий узел подсвечивается (`SetSelected(true)`), отрисовка инициируется через `environment->Repaint(QRect())`.

## 5.4. Выбор перехода (`FindNextMachine`)

```cpp
char sign = _line->GetValueAtMachine();         // символ под головкой
for (transition : machine->GetOutgoingTransitions())
    if (transition->GetConditions().IsEnabled(sign))
        return { same_complex, transition->GetFinishMachine() };
return current_state;                            // нет подходящего — остаёмся
```

Корректность диаграммы (полнота алфавита, отсутствие дубликатов) проверяется заранее в `FormCompilerMachine` (см. ниже).

## 5.5. Пошаговый и непрерывный режимы

### Пошаговый режим
Кнопка `button_debug_step` → `FormDebugerMachine::Step()` → `_debugger.Step(this)` + перерисовка ленты.

### Непрерывный режим
Кнопка `button_debug_run` → `FormDebugerMachine::Run()`:

```cpp
void RunTimer() {
    if (!_debugger.IsFinish() && !_stop) {
        Step();
        _timer.singleShot(50, this, SLOT(RunTimer()));  // следующий тик
    } else {
        // включить debug_step / debug_run, выключить debug_stop;
        // если IsTestMode — CheckTest()
    }
}
```

Шаги выполняются в очереди событий Qt каждые 50 мс — UI остаётся отзывчивым, можно нажать `button_debug_stop` (`_stop = true`) для прерывания.

`IsFinish()` истинно только когда стек пуст **и** текущий узел — `MT_FINISH` корневой машины.

## 5.6. Проверки корректности (compile-time)

Перед запуском диаграмма прогоняется через `FormCompilerMachine` (`formcompilermachine.cpp`):

| Проверка | Описание |
|---|---|
| `CheckStart` | ровно один `MT_START` в каждой комплексной машине. |
| `CheckFinish` | хотя бы один `MT_FINISH`. |
| `CheckConnection` | у `MT_START` нет входящих, у `MT_FINISH` нет исходящих; у всех остальных — есть и те, и другие. |
| `CheckAllCases` | объединение условий исходящих переходов покрывает весь алфавит. |
| `CheckDublicate` | один символ алфавита не должен встречаться в условиях двух разных исходящих переходов одного узла. |

Если есть ошибки — выводится список с возможностью «допрыгнуть» к проблемному узлу; иначе пользователь попадает в отладчик.

## 5.7. Режим упражнений

`VMTProject::IsTestMode()` включает специальный поток в `FormDebugerMachine`:

1. `StartExercise()` → `PrepareTest()` загружает вход первого теста на ленту (`SetSignAtMachine` + `ShiftRight`).
2. `Run()` выполняет программу до `MT_FINISH`.
3. `CheckTest()` сравнивает финальное содержимое ленты с ожидаемой строкой:
   - совпадает → следующий тест;
   - не совпадает → красный маркер, сообщение «Test failed».
4. После прохождения всех тестов упражнение помечается `completed=true` в `Configuration`.
