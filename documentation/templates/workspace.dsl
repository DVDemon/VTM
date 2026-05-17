workspace "Пример системы" "Демонстрация всех правил workspace.dsl" {


    !identifiers hierarchical

    model {
        # [Регистрация в репозитории] workspace_cmdb — мнемоника приложения для публикации
        properties {
            workspace_cmdb "EXAMPLE"
        }

        # [3.1 Контекстная диаграмма] Внешние системы с property cmdb
        ext_idp = softwareSystem "IDP" {
            tags "landscape"
            properties {
                cmdb "IDP"
            }
        }

        # [3.1] Пользователи (Person)
        user = person "Абонент"
        admin = person "Администратор"

        # [Регистрация, 3.1, 3.2] Основная система — cmdb на уровне системы
        example = softwareSystem "Example System" "Пример приложения" {
            properties {
                cmdb "EXAMPLE"
            }

            # [3.2 Контейнерная диаграмма] Контейнер с tags, technology, url, external_name
            api = container "Backend API" "REST API сервис" {
                tags "springboot"
                technology "Spring Boot, Java 17"
                url "https://git.vimpelcom.ru/products/example/backend-api"
                properties {
                    external_name ext_example_api
                }

                # [2 Определение перечня ТС] Technical Capability — component с type capability
                component "Получение данных профиля" {
                    description "Возможность получать информацию о профиле пользователя"
                    properties {
                        type        capability
                        code        001
                        parents     "BC-011590"
                        version     "1.0"
                    }
                }

                # [3.6 Нефункциональные требования] API Component — type api, api_url, protocol, НФТ
                component "profile_api" "REST API профиля" {
                    properties {
                        type          api
                        api_url       "https://api.example.com/openapi.yaml"
                        protocol      rest
                        tc            BC-011590
                        "get /profile"    rps=100;latency=50;error_rate=0.01
                        "put /profile"    rps=10;latency=100;error_rate=0.05
                    }
                }
            }

            # [3.2, 3.8] Контейнер БД с entity (бизнес-сущности из глоссария DMP)
            db = container "Profile DB" "База данных профилей" {
                tags "database" "postgresql"
                technology "PostgreSQL 14"
                url "https://git.vimpelcom.ru/products/example/db-migrations"
                properties {
                    entity "UserProfile, UserPreferences"
                }
            }

            # [3.2] Связи с протоколом и портом; [3.8] send/return для данных
            api -> db "Чтение/запись профиля" "TCP:5432" {
                properties {
                    send   "UserProfile"
                    return "UserProfile, UserPreferences"
                }
            }
        }

        # [3.1] Связи контекста — направление вызова, подпись
        user -> example.api "Использует"
        admin -> example "Администрирует"
        example.api -> ext_idp "Аутентификация" "HTTPS:443"


        # [4.2 Диаграмма развертывания] deploymentEnvironment, deploymentNode, containerInstance
        deploymentEnvironment "Production" {
            deploymentNode "PROTECTED STD OPS" "Сегмент" {
                deploymentNode "example-ns" "K8s Namespace" {
                    properties { 
                        type "k8s-ns" 
                    }

                    deploymentNode "example-api" {
                        properties { 
                            type "pod"
                            cpu  "1"
                            ram  "2"
                        }

                        containerInstance example.api
                    }
                }
                deploymentNode "db.example.cloud"  {
                    properties {
                        type         "vm"
                        flavor       "cpu2ram8"
                        volume_size  "50"
                    }
                    containerInstance example.db
                }
            }
        }
    }


    views {
        # [3.8 Логическая модель данных] Kroki для DBML-диаграмм
        properties {
            kroki.url    "https://kroki.io"
            kroki.format "svg"
        }

        

        # [3.1] Контекстная диаграмма C1
        systemContext example {
            include *
            autoLayout lr
        }

        # [3.2] Контейнерная диаграмма C2
        container example {
            include *
            autoLayout lr
        }

        # [4.1] Компонентная диаграмма C3
        component example.api {
            include *
            autoLayout
        }

        # [3.5 Сценарии использования] Dynamic View — ключ = код technical capability
        dynamic example "001" {
            autoLayout
            title "Получение профиля пользователя"
            user -> example.api "Запрос профиля\n GET /profile"
            example.api -> example.db "Получение данных\n SELECT * FROM profiles"
            example.api -> ext_idp "Проверка токена\n GET /auth/verify"
        }

        # [4.2] Диаграмма развертывания
        deployment example "Production" {
            include *
            autoLayout
        }

        # [3.8] Пример Image View с DBML (раскомментировать при наличии datamodel.dbml)
        # image example "domain" {
        #     kroki "dbml" "./datamodel.dbml"
        # }
    }
}
