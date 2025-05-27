#include "audiobackend.h"
#include "audiochannel.h"
#include "audioerror.h"
#include "bass.h"

#include <iostream>

#include <QDebug>
#include <QTimer>

#define BASS_NO_FLAGS 0

AudioBackend::AudioBackend(QObject *parent)
    : QObject{parent}

{
  BASS_SetConfig(BASS_CONFIG_DEV_DEFAULT, 1);
  BASS_Free();
  BASS_PluginLoad("bassopus", BASS_NO_FLAGS);
  qDebug() << "Status of BASSOPUS plugin:" << AudioError::getErrorMessage();

  DWORD device_handle;
  BASS_DEVICEINFO info;

  for (device_handle = 0; BASS_GetDeviceInfo(device_handle, &info); device_handle++)
  {
    if (BASS_Init(device_handle, 48000, BASS_DEVICE_LATENCY, nullptr, nullptr))
    {
      qDebug() << "Initialised device" << info.name;
    }
    else
    {
      qDebug() << "Failed to initialise device" << info.name;
    }
  }

  channels.insert(0, new AudioChannel(0, 1, this));
  channels[0]->setFile("https://attorneyoffline.de/vanillabase/sounds/music/ace%20attorney/objection/%5baj%5d%20objection.opus");
  channels[0]->setChannelVolume(25);
  channels[0]->start();

  QTimer *timer = new QTimer();
  timer->setSingleShot(true);
  timer->setInterval(5000);
  timer->callOnTimeout([this]() {
    std::cout << "Slot called" << std::endl;
    channels[0]->stop();
  });
  timer->start();

  QTimer *timer2 = new QTimer();
  timer2->setSingleShot(true);
  timer2->setInterval(10000);
  timer2->callOnTimeout([this]() {
    std::cout << "Slot 2 called" << std::endl;
    channels[0]->start();
  });
  timer2->start();
}

AudioBackend::~AudioBackend()
{}
