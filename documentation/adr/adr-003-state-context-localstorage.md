# ADR-003: Состояние и хранилище — React Context + localStorage

Дата: 2026-05-17

Статус: Accepted

## Контекст

Десктоп VMT хранит проект в бинарном `.jdtp` (см. [`../data-formats.md`](../data-formats.md)) и в синглтоне `VMTProject`. В вебе:

- нет бэкенда (NFR-002) и аутентификации (NFR-003);
- нужно сохранять проекты, недавние, прогресс упражнений, настройки;
- модель `Project` иммутабельная (см. ADR-005), Undo — через стек снапшотов;
- размер «среднего учебного» проекта — десятки KB.

Рассматривались: React Context + localStorage, Zustand + IndexedDB, Redux Toolkit + IndexedDB.

Требования: FR-001 (CRUD проекта), FR-017 (прогресс упражнений), FR-018 (Undo), FR-022 (recent), NFR-002, NFR-003, NFR-008.

## Решение

Состояние и хранилище:

- **In-memory state** — React Context (`ProjectContext`, `EditorContext`, `DebuggerContext`, `SettingsContext`). Каждое — небольшая обёртка `useReducer` + `Provider`. Подписки — через узкие селекторы (`useContextSelector`-паттерн или ручное мемоизирование) для предотвращения каскадных ререндеров.
- **Persistence черновика** — `localStorage` под префиксом `vmt:` для **строкового JSON** черновика проекта (`.vmt.json`), списка «recent», прогресса упражнений, настроек. Сохранение — debounced (300 мс). **Бинарные файлы `.jdtp` в localStorage не сохраняются** — только загрузка с диска и сохранение на диск через файловый API/UI (ADR-005).
- **Лимит**: до 4 MB на проект. При превышении — модалка «Project too large for browser storage» с предложением экспортировать в файл (`Export as .json`).
- **Undo/Redo** — стек снапшотов в памяти (не персистится), глубина 50.

Никаких глобальных state-менеджеров не вводим в V1.

## Альтернативы

### Альтернатива 1: Zustand + IndexedDB (idb)

Преимущества: нет 5 MB лимита, поддержка бинарных blob, более удобный store. Минусы: + 2 зависимости в бандл, нужна асинхронная инициализация (loading state), сложнее тестировать. Для V1 нет проектов > 4 MB — выгода не оправдывает сложность. Решение: оставить как путь миграции для V1.1 (см. §10–11 в `analysis.md`).

### Альтернатива 2: Redux Toolkit + IndexedDB

Тяжелее по бойлерплейту, аналогичные плюсы/минусы по storage. Для команды из 1–2 человек на учебном инструменте оверкилл.

### Альтернатива 3: Только Context без persistence

Простейший вариант, но теряет требование FR-001 (сохранение проекта между визитами) и FR-022 (recent).

## Последствия

Положительные:

- Нулевые внешние зависимости для state.
- Простая отладка: state видно в React DevTools, persisted-данные — в DevTools → Application → Local Storage.
- Понятная стратегия миграции: ключи под префиксом легко перенести на IndexedDB в V1.1.

Отрицательные / компромиссы:

- 4–5 MB лимит localStorage; крупные проекты придётся выгружать в файл (NFR обязывает к explicit UX).
- localStorage — синхронный API: при больших write вижу UI-jank → используем debounce и сериализацию только дельты «текущий проект».
- Без `useContextSelector` каскадные ререндеры; договариваемся писать чистые селекторы или взять `use-context-selector` (одна маленькая зависимость).
- localStorage уязвим к XSS → строгая CSP обязательна (см. ADR-001 + Safeguards в Canvas).

## Проверка соблюдения

- В `package.json` нет `zustand`, `redux`, `mobx` (state-менеджеры) — только React + опционально `use-context-selector`.
- В UI нет вызова `localStorage.setItem` для бинарных данных или для содержимого `.jdtp`.
- В `src/storage/` есть `PersistedStorage`-обёртка с защитой `try/catch` и проверкой `QuotaExceededError`.
- Юнит-тест проверяет «save → reload → load» для эталонного проекта.
- Юнит-тест проверяет обработку `QuotaExceededError` (mock localStorage).
