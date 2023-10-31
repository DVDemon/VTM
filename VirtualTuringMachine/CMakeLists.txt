cmake_minimum_required(VERSION 3.10)

set(CMAKE_PREFIX_PATH ${QTDIR})

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

set(ADDITIONAL_DIRS ${CMAKE_CURRENT_SOURCE_DIR} interfaces vmtactions
                    vmtexport4th vmtmachines)
include_directories(${ADDITIONAL_DIRS})

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
    add_executable(${PROJECT_NAME} resources.qrc ${SOURCES}
        isCtrlPressed.h)
  endif()
endif()

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE Qt${QT_VERSION_MAJOR}::Core
  PRIVATE Qt${QT_VERSION_MAJOR}::Gui
  PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

qt_import_plugins(
  ${PROJECT_NAME}
  INCLUDE
  Qt::Qt5::QCocoaIntegrationPlugin
  Qt::Qt5::QGifPlugin
  Qt::Qt5::QICNSPlugin
  Qt::Qt5::QICOPlugin
  Qt::Qt5::QJpegPlugin
  Qt::QMacJp2Plugin
  Qt::QTgaPlugin
  Qt::QTiffPlugin
  Qt::QWbmpPlugin
  Qt::QWebpPlugin)

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(${PROJECT_NAME})
endif()
