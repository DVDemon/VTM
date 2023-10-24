cmake_minimum_required(VERSION 3.10)

set(CMAKE_PREFIX_PATH ${QTDIR})

set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets Core5Compat)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets
                                                       Core5Compat)

set(INTERFACES_DIR interfaces vmtactions vmtexport4th vmtmachines)
include_directories(${INTERFACES_DIR})

file(
  GLOB_RECURSE SOURCES
  LIST_DIRECTORIES FALSE
  *.cpp)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(${PROJECT_NAME} resources.qrc MANUAL_FINALIZATION
                    ${SOURCES})
else()
  if(ANDROID)
    add_library(${PROJECT_NAME} resources.qrc SHARED ${SOURCES})
  else()
    add_executable(${PROJECT_NAME} resources.qrc ${SOURCES})
  endif()
endif()

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE Qt${QT_VERSION_MAJOR}::Core
  PRIVATE Qt${QT_VERSION_MAJOR}::Gui
  PRIVATE Qt${QT_VERSION_MAJOR}::Core5Compat
  PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(${PROJECT_NAME})
endif()
