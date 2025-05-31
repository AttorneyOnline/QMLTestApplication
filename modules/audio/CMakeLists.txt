cmake_minimum_required(VERSION 3.16)

qt_add_qml_module(AudioModule
  STATIC
URI Attorney.Audio
VERSION 1.0
  SOURCES
  src/audio.h src/audio.cpp
  src/audiobackend.h src/audiobackend.cpp
  src/audiochannel.h src/audiochannel.cpp
  src/audioerror.h
  SOURCES src/audiosettings.h src/audiosettings.cpp
)

target_include_directories(AudioModule PUBLIC src libs)
target_link_directories(AudioModule PUBLIC libs)
target_link_libraries(AudioModule PUBLIC
  Qt6::Core
  Qt6::Qml
  bass
  bassopus
)
