add_subdirectory(audio)

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
