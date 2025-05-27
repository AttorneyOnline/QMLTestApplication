#include "audiobackend.h"
#include "audiochannel.h"
#include "audioerror.h"
#include "bass.h"

#include <QDebug>
#include <QVariant>

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
  channels[0]->setFile("Cross-Examination - Allegro 2001 - AA.opus");
  channels[0]->setLoopPoints(QVariant("21.269").toDouble(), QVariant("78.582").toDouble());
  channels[0]->setChannelVolume(25);
  channels[0]->start();
}

AudioBackend::~AudioBackend()
{}

void AudioBackend::setChannelVolume(int channel, int volume)
{
  channels[channel]->setChannelVolume(volume);
}
