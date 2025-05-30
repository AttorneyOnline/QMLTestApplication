cmake_minimum_required(VERSION 3.16)

project(QMLTestApplication LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Quick Qml)
find_package(Qt6 REQUIRED COMPONENTS Core Quick Qml)

set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml)
set(QML_IMPORT_PATH ${QT_QML_OUTPUT_DIRECTORY} CACHE PATH "Extra QML import paths to make Qt Creator happy")
qt_policy(SET QTP0001 NEW)

add_subdirectory(src/audiomodule)
add_subdirectory(src/settingsmodule)

qt_standard_project_setup(REQUIRES 6.8)
qt_add_executable(QMLTestApplication
  src/main.cpp
  ressources/main.qml
)

target_link_libraries(QMLTestApplication PRIVATE
  Qt6::Core
  Qt6::Quick
)

set_target_properties(QMLTestApplication PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_CURRENT_LIST_DIR}/bin>
        RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_CURRENT_LIST_DIR}/bin>)
