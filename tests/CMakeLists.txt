set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Test)

add_executable(test_machine_serialization
               serialization/test_machine_serialization.cpp)

target_link_libraries(test_machine_serialization
                      PRIVATE vtm_core Qt5::Test)

add_test(NAME test_machine_serialization
         COMMAND test_machine_serialization)
