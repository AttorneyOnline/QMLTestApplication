cmake_minimum_required(VERSION 3.16)

qt_add_qml_module(SettingsModule
  STATIC
URI Attorney.Settings
VERSION 1.0
SOURCES
    src/settings.h
    src/settings.cpp
    src/settingsbackend.h
    src/settingsbackend.cpp
)
target_link_libraries(SettingsModule PRIVATE
  Qt6::Core
)
target_include_directories(SettingsModule PRIVATE src)
