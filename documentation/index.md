# Virtual Turing Machine — документация

VTM (Virtual Turing Machine) — кросс-платформенное Qt-приложение для визуального проектирования, отладки и исполнения программ для машин Тьюринга. Программа представлена в виде диаграммы состояний, машины могут вкладываться друг в друга и переиспользоваться, есть пошаговый и непрерывный режим интерпретации, экспорт в текстовое (четвёрки), графическое (PNG) и UML-представление (PlantUML).

Документ описывает функциональные и нефункциональные требования и архитектуру решения.

## Разделы

| # | Раздел | Файл |
|---|--------|------|
| 1 | Общее назначение системы | [`overview.md`](overview.md) |
| 2 | Формы и пользовательские сценарии | [`forms-and-scenarios.md`](forms-and-scenarios.md) |
| 3 | Форматы сохраняемых и читаемых данных | [`data-formats.md`](data-formats.md) |
| 4 | Форматы экспорта (четвёрки, PlantUML, PNG) | [`export-formats.md`](export-formats.md) |
| 5 | Принцип работы интерпретатора | [`interpreter.md`](interpreter.md) |
| 6 | Внутреннее устройство машин Тьюринга | [`machines.md`](machines.md) |
| 7 | Принцип отрисовки диаграмм и связей | [`rendering.md`](rendering.md) |

## Спецификация веб-версии (Specification First + SPDD)

| Артефакт | Файл |
|---|---|
| Анализ и план первой итерации | [`prompts/analysis.md`](prompts/analysis.md) |
| REASONS Canvas (V1) | [`prompts/reasons-canvas.md`](prompts/reasons-canvas.md) |
| Архитектура C4 в Structurizr DSL | [`workspace.dsl`](workspace.dsl) |
| ADR-001 — Frontend SPA: Vite + React + TS | [`adr/adr-001-frontend-spa-vite-react.md`](adr/adr-001-frontend-spa-vite-react.md) |
| ADR-002 — Холст: react-konva | [`adr/adr-002-canvas-react-konva.md`](adr/adr-002-canvas-react-konva.md) |
| ADR-003 — State + Storage: Context + localStorage | [`adr/adr-003-state-context-localstorage.md`](adr/adr-003-state-context-localstorage.md) |
| ADR-004 — Маршрутизация: A* + Безье | [`adr/adr-004-routing-astar-bezier.md`](adr/adr-004-routing-astar-bezier.md) |
| ADR-005 — Формат проекта: JSON v1 + обмен `.jdtp` через диск | [`adr/adr-005-project-format-json.md`](adr/adr-005-project-format-json.md) |
| ADR-006 — Без аутентификации в V1 | [`adr/adr-006-no-auth.md`](adr/adr-006-no-auth.md) |
| ADR-007 — Деплой на GitHub Pages | [`adr/adr-007-deploy-github-pages.md`](adr/adr-007-deploy-github-pages.md) |

## Нефункциональные требования (сводка)

- **Платформы:** Windows, macOS (x86_64, arm64), Linux, Android, iOS.
- **Технологии:** C++17, Qt 5.15+ или Qt 6.5+, сборка через qmake или CMake (3.16+).
- **Локализация:** UTF-8 (русский и английский интерфейс упражнений).
- **Производительность UI:** интерактивное редактирование диаграмм с зумом, перерисовка только изменившихся областей.
- **Совместимость форматов:** поддержка legacy-формата проекта (`.jdtp` v1) и текущего v2 без потерь данных.
- **Тестируемость:** модуль `vtm_core` собирается как статическая библиотека и покрывается unit-тестами на Qt Test (сериализация, pathfinder).
- **Без побочных зависимостей** для исполнения — поставка одним бинарником/бандлом, упражнения встроены в ресурсы.
