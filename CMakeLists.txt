cmake_minimum_required(VERSION 3.16)

project(QMLTestApplication VERSION 1.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(QT_QML_GENERATE_QMLLS_INI OFF) #Apparently an important thing?

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Quick Qml Widgets QuickControls2)
find_package(Qt6 REQUIRED COMPONENTS Core Quick Qml Widgets QuickControls2)

set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml)
set(QML_IMPORT_PATH ${QT_QML_OUTPUT_DIRECTORY} CACHE PATH "Extra QML import paths to make Qt Creator happy")
qt_policy(SET QTP0001 NEW)

add_subdirectory(modules)

qt_standard_project_setup(REQUIRES 6.8)
qt_add_executable(QMLTestApplication
  src/main.cpp
  bin/files/system/main.qml
)

target_link_libraries(QMLTestApplication PRIVATE
  Qt6::Core
  Qt6::Quick
  Qt6::Widgets
  AudioModuleplugin
  ThemeModuleplugin
  KDAB::kddockwidgets
)

set_target_properties(QMLTestApplication PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_CURRENT_LIST_DIR}/bin>
        RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_CURRENT_LIST_DIR}/bin>
)

if(WIN32)
    set_target_properties(QMLTestApplication PROPERTIES
        LINK_FLAGS_RELEASE "/SUBSYSTEM:CONSOLE /ENTRY:mainCRTStartup"
)
endif(WIN32)
