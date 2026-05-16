cmake_minimum_required(VERSION 3.16)

if(NOT CMAKE_PREFIX_PATH AND DEFINED ENV{QTDIR})
  list(APPEND CMAKE_PREFIX_PATH "$ENV{QTDIR}")
endif()

if(APPLE AND NOT CMAKE_PREFIX_PATH)
  set(_qt5_mac_candidates
      "$ENV{HOME}/Qt/5.15.2/clang_64"
      "/opt/homebrew/opt/qt@5"
      "/usr/local/opt/qt@5")
  foreach(_qt_path IN LISTS _qt5_mac_candidates)
    if(EXISTS "${_qt_path}/lib/cmake/Qt5/Qt5Config.cmake")
      list(APPEND CMAKE_PREFIX_PATH "${_qt_path}")
      break()
    endif()
  endforeach()
endif()

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

if(APPLE)
  get_target_property(_qt_core_location Qt5::Core LOCATION)
  if(_qt_core_location)
    get_filename_component(_qt_lib_dir "${_qt_core_location}" DIRECTORY)
    execute_process(
      COMMAND lipo -info "${_qt_lib_dir}/QtCore"
      OUTPUT_VARIABLE _qt_lipo_info
      ERROR_QUIET
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    if(_qt_lipo_info MATCHES "Non-fat file: .* architecture: x86_64"
       AND CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64"
       AND NOT CMAKE_OSX_ARCHITECTURES)
      set(CMAKE_OSX_ARCHITECTURES
          x86_64
          CACHE STRING "Qt 5.15 macOS kit is x86_64-only; build via Rosetta on Apple Silicon."
                FORCE)
      message(STATUS "Qt is x86_64-only: setting CMAKE_OSX_ARCHITECTURES=x86_64")
    endif()
  endif()
endif()

set(ADDITIONAL_DIRS ${CMAKE_CURRENT_SOURCE_DIR} interfaces vmtactions
                    vmtexport4th vmtexportplantuml vmtmachines)
include_directories(${ADDITIONAL_DIRS})

file(
  GLOB_RECURSE VTM_SOURCES
  LIST_DIRECTORIES FALSE
  *.cpp)
list(FILTER VTM_SOURCES EXCLUDE REGEX "/main\\.cpp$")

add_library(vtm_core STATIC ${VTM_SOURCES} resources.qrc)
target_include_directories(vtm_core PUBLIC ${ADDITIONAL_DIRS})
target_link_libraries(
  vtm_core
  PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui
         Qt${QT_VERSION_MAJOR}::Widgets)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(${PROJECT_NAME} main.cpp MANUAL_FINALIZATION)
else()
  if(ANDROID)
    add_library(${PROJECT_NAME} SHARED main.cpp)
  else()
    add_executable(${PROJECT_NAME} main.cpp isCtrlPressed.h)
  endif()
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE vtm_core)

if(APPLE AND NOT ANDROID)
  set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE ON)
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/MyAppInfo.plist")
    set_target_properties(
      ${PROJECT_NAME}
      PROPERTIES MACOSX_BUNDLE_INFO_PLIST
                 "${CMAKE_CURRENT_SOURCE_DIR}/MyAppInfo.plist")
  endif()
endif()

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(${PROJECT_NAME})
endif()
