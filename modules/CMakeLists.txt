cmake_minimum_required(VERSION 3.16)

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)

qt_add_qml_module(AudioModule
  STATIC
URI Attorney.Audio
VERSION 1.0
  SOURCES
  audio/src/audio.h audio/src/audio.cpp
  audio/src/audiobackend.h audio/src/audiobackend.cpp
  audio/src/audiochannel.h audio/src/audiochannel.cpp
  audio/src/audioerror.h
  audio/src/audiosettings.h audio/src/audiosettings.cpp
)

target_include_directories(AudioModule PUBLIC audio/src audio/libs)
target_link_directories(AudioModule PUBLIC audio/libs)
target_link_libraries(AudioModule PUBLIC
  Qt6::Core
  Qt6::Qml
  bass
  bassopus
)

qt_add_qml_module(ThemeModule
  STATIC
URI Attorney.Theme
VERSION 1.0
  SOURCES
  theme/src/theme.h theme/src/theme.cpp
  theme/src/themebackend.h theme/src/themebackend.cpp
  theme/src/themesettings.h theme/src/themesettings.cpp
)

target_include_directories(ThemeModule PUBLIC theme/src)
target_link_libraries(ThemeModule PUBLIC
  Qt6::Core
  Qt6::Qml
)
target_link_libraries(AudioModule PRIVATE Qt6::Core)
target_link_libraries(AudioModule PRIVATE Qt6::Core)

set(KDDockWidgets_QT6 ON CACHE BOOL "Enable KDDockWidgets Qt6 Support")
set(KDDockWidgets_FRONTENDS CACHE STRING "qtquick")
set(KDDockWidgets_QTQUICK ON CACHE BOOL "Enable Qt Quick modules in KDDockWidgets" FORCE)
set(KDDockWidgets_EXAMPLES OFF CACHE BOOL "Disable examples in KDDockWidgets" FORCE)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/bin CACHE PATH "Install prefix" FORCE)

include(FetchContent)
FetchContent_Declare(
  KDDockWidgets
  GIT_REPOSITORY https://github.com/KDAB/KDDockWidgets.git
  GIT_TAG        v2.2.5
  SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/kddockwidgets
)
FetchContent_MakeAvailable(KDDockWidgets)

set_target_properties(kddockwidgets PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/$<0:>
)
