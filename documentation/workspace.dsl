workspace "VMT Web" "Веб-версия Virtual Turing Machine: визуальный редактор и интерпретатор машин Тьюринга в браузере" {

    !identifiers hierarchical

    model {
        properties {
            workspace_cmdb "VMT"
        }

        github_pages = softwareSystem "GitHub Pages" {
            tags "landscape"
            properties {
                cmdb "GITHUB_PAGES"
            }
        }

        browser_storage = softwareSystem "Browser Storage" "Локальное хранилище браузера (localStorage для строк .vmt.json; без бинарных .jdtp)" {
            tags "landscape" "database"
            properties {
                cmdb "BROWSER_STORAGE"
            }
        }

        user_fs = softwareSystem "Файловая система пользователя" "Локальные файлы на устройстве (.jdtp, .vmt.json и др.)" {
            tags "landscape" "filesystem"
            properties {
                cmdb "USER_FS"
            }
        }

        student    = person "Студент" "Создаёт диаграммы машин Тьюринга, решает упражнения"
        instructor = person "Преподаватель" "Готовит упражнения, проверяет решения студентов"

        vmt_web = softwareSystem "VMT Web" "Веб-приложение: визуальный редактор машин Тьюринга, отладчик, экспорты" {
            properties {
                cmdb "VMT"
            }

            spa = container "SPA Shell" "Точка входа, layout, маршрутизация (HashRouter), файловые диалоги, i18n" {
                tags "react" "spa"
                technology "Vite, React 18, TypeScript"
                url "https://github.com/dvdemon/VTM"
                properties {
                    external_name "ext_vmt_spa"
                }

                component "App Router" {
                    description "HashRouter, layout, ленивые экраны Main/Editor/Compiler/Debugger/Exercises/Export4th"
                    properties {
                        type        capability
                        code        "000"
                        version     "1.0"
                    }
                }

                component "i18n" "Переключатель RU/EN, ресурсы переводов" {
                    properties {
                        type        capability
                        code        "012"
                        version     "1.0"
                    }
                }
            }

            editor = container "Editor / Canvas" "Визуальный редактор диаграммы: сцена, узлы, связи, drag/zoom" {
                tags "react" "canvas"
                technology "React + react-konva (Canvas 2D), 4 слоя"
                url "https://github.com/dvdemon/VTM"

                component "Stage Renderer" "Сцена Konva: фон/сетка, рёбра, узлы, overlay" {
                    properties {
                        type        capability
                        code        "003"
                        version     "1.0"
                    }
                }

                component "DragDrop" "Перемещение, добавление, удаление узлов; snap-to-grid" {
                    properties {
                        type        capability
                        code        "031"
                        version     "1.0"
                    }
                }

                component "Conditions Editor" "Редактор условий перехода (символы алфавита, *, ε)" {
                    properties {
                        type        capability
                        code        "006"
                        version     "1.0"
                    }
                }

                component "Tape View" "Виджет отображения бесконечной ленты в отладчике" {
                    properties {
                        type        capability
                        code        "010"
                        version     "1.0"
                    }
                }
            }

            core = container "Core (Domain)" "Доменная модель, интерпретатор, маршрутизация, сериализаторы, экспорты" {
                tags "domain"
                technology "TypeScript (strict), без React/DOM-зависимостей"
                url "https://github.com/dvdemon/VTM"

                component "Domain Model" "Project, ComplexMachine, MachineBody, Transition, Alphabet, AlphaMachine" {
                    description "Иммутабельные структуры предметной области"
                    properties {
                        type        capability
                        code        "001"
                        version     "1.0"
                    }
                }

                component "Interpreter" "Debugger: пошаговый и непрерывный режим, стек вложенных вызовов, выбор перехода" {
                    properties {
                        type        capability
                        code        "002"
                        version     "1.0"
                    }
                }

                component "Compiler Checks" "5 проверок: Start/Finish/Connection/AllCases/Dublicate" {
                    properties {
                        type        capability
                        code        "009"
                        version     "1.0"
                    }
                }

                component "Pathfinder" "A* по сетке со штрафом за поворот, simplify, ortho, terminal directions" {
                    properties {
                        type        capability
                        code        "007"
                        version     "1.0"
                    }
                }

                component "Bezier Renderer" "Catmull-Rom -> cubic Bezier, flatten, isPointNearStroke, snapPortHeights" {
                    properties {
                        type        capability
                        code        "008"
                        version     "1.0"
                    }
                }

                component "Serializer JSON" "Сохранение/загрузка нативного формата .vmt.json (schemaVersion + миграции)" {
                    properties {
                        type        capability
                        code        "011"
                        version     "1.0"
                    }
                }

                component "Serializer JDTP" "Кодирование и декодирование бинарных .jdtp v1/v2 (совместимость с десктопным QDataStream)" {
                    properties {
                        type        capability
                        code        "020"
                        version     "1.0"
                    }
                }

                component "Exporter PNG" "OffscreenCanvas + toBlob; рендер той же сцены" {
                    properties {
                        type        capability
                        code        "014"
                        version     "1.0"
                    }
                }

                component "Exporter PlantUML" "Генерация .puml State Machine: nested state, [*], метки * / epsilon" {
                    properties {
                        type        capability
                        code        "015"
                        version     "1.0"
                    }
                }

                component "Exporter Fourth" "Нотация четверок: state | char | cmd | next; проверка рекурсии" {
                    properties {
                        type        capability
                        code        "016"
                        version     "1.0"
                    }
                }

                component "Tape" "Бесконечная двусторонняя лента, блоки по 10 символов (порт VMTLine)" {
                    properties {
                        type        capability
                        code        "010b"
                        version     "1.0"
                    }
                }

                component "Undo Manager" "Стек снапшотов Project, глубина 50" {
                    properties {
                        type        capability
                        code        "018"
                        version     "1.0"
                    }
                }
            }

            storage = container "Storage" "React Context + localStorage: черновики .vmt.json, recent, settings, exercise progress" {
                tags "storage"
                technology "TypeScript, React Context, localStorage (ключи под префиксом vmt:)"
                properties {
                    entity "ProjectJsonDraft, RecentList, ExerciseProgress, Settings"
                }
            }

            exercises_resource = container "Exercises Resource" "Встроенный JSON с упражнениями (мигрирован из exercises.xml)" {
                tags "static"
                technology "Static JSON в бандле"
            }

            spa -> editor "Рендерит редактор и отладчик"
            spa -> core "Вызывает доменные операции" {
                properties {
                    send   "Project, EditorAction"
                    return "Project, ValidationResult, ExportArtifact"
                }
            }
            spa -> storage "Читает/пишет черновики и recent" {
                properties {
                    send   "ProjectJsonDraft, Settings"
                    return "ProjectJsonDraft, RecentList, Settings"
                }
            }
            spa -> exercises_resource "Загружает упражнения" {
                properties {
                    return "Exercise[]"
                }
            }
            spa -> user_fs "Открытие и сохранение файлов проекта (.jdtp, .vmt.json)" "File API / download"

            editor -> core "Использует доменные операции и построение маршрутов связей" {
                properties {
                    send   "Machine, Transition, EditorIntent"
                    return "Project, RoutingPolyline, BezierPath"
                }
            }
        }

        student -> vmt_web.spa "Использует редактор и отладчик" "HTTPS:443"
        instructor -> vmt_web.spa "Готовит упражнения, проверяет решения" "HTTPS:443"
        vmt_web.spa -> github_pages "Раздаётся как статическая сборка" "HTTPS:443"
        vmt_web.storage -> browser_storage "persist строк JSON и метаданные" "localStorage API"


        deploymentEnvironment "Production" {

            deploymentNode "Рабочее место пользователя" "Локальный компьютер" {

                deploymentNode "Браузер" "Chrome / Firefox / Safari" {
                    properties {
                        type "browser"
                    }

                    deploymentNode "Вкладка SPA" {
                        properties {
                            type "browser-tab"
                        }

                        containerInstance vmt_web.spa
                        containerInstance vmt_web.editor
                        containerInstance vmt_web.core
                        containerInstance vmt_web.storage
                        containerInstance vmt_web.exercises_resource
                    }
                }
            }
        }
    }


    views {
        properties {
            kroki.url    "https://kroki.io"
            kroki.format "svg"
        }

        systemContext vmt_web {
            include *
            autoLayout lr
        }

        container vmt_web {
            include *
            autoLayout lr
        }

        component vmt_web.spa {
            include *
            autoLayout
        }

        component vmt_web.editor {
            include *
            autoLayout
        }

        component vmt_web.core {
            include *
            autoLayout
        }

        dynamic vmt_web "001" {
            autoLayout lr
            title "Открытие сохранённого черновика (.vmt.json)"
            student -> vmt_web.spa "Открывает приложение"
            vmt_web.spa -> vmt_web.storage "Читает ProjectJsonDraft из localStorage"
            vmt_web.spa -> vmt_web.core "Десериализует JSON в модель"
            vmt_web.spa -> vmt_web.editor "Отображает диаграмму на холсте"
        }

        dynamic vmt_web "002" {
            autoLayout lr
            title "Пошаговое исполнение (Debugger.step)"
            student -> vmt_web.spa "Нажимает Step"
            vmt_web.spa -> vmt_web.editor "Фокус на режиме отладки"
            vmt_web.editor -> vmt_web.core "Interpreter.step(state)"
        }

        dynamic vmt_web "015" {
            autoLayout lr
            title "Экспорт в PlantUML"
            student -> vmt_web.spa "Нажимает Export PUML"
            vmt_web.spa -> vmt_web.editor "Запрос текущей диаграммы"
            vmt_web.editor -> vmt_web.core "Exporter PlantUML.build(Project)"
        }

        dynamic vmt_web "020" {
            autoLayout lr
            title "Открытие файла .jdtp с локального диска"
            student -> vmt_web.spa "Выбирает Open .jdtp"
            vmt_web.spa -> user_fs "Диалог выбора файла; чтение ArrayBuffer"
            vmt_web.spa -> vmt_web.core "Serializer JDTP.decode(buffer) -> Project"
            vmt_web.spa -> vmt_web.editor "Отображает загруженную диаграмму"
        }

        dynamic vmt_web "023" {
            autoLayout lr
            title "Сохранение диаграммы как .jdtp на локальный диск"
            student -> vmt_web.spa "Нажимает Save .jdtp"
            vmt_web.spa -> vmt_web.core "Serializer JDTP.encode(Project) -> ArrayBuffer"
            vmt_web.spa -> user_fs "Скачивание файла (Blob / save-file picker)"
        }

        deployment vmt_web "Production" {
            include *
            autoLayout lr
        }
    }
}
