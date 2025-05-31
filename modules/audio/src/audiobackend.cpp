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
  BASS_Free();
  BASS_SetConfig(BASS_CONFIG_DEV_DEFAULT, 1);
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
}

AudioBackend::~AudioBackend()
{}

void AudioBackend::setChannelSong(int id, QString song)
{
  AudioChannel *old_channel = channels.value(id);
  if (old_channel)
  {
    old_channel->deleteLater();
  }

  AudioChannel *channel = new AudioChannel(id, 2, this);
  channels.insert(id, channel);
  channel->setFile(song);
  channel->setChannelVolume(25);
  channel->start();
}

void AudioBackend::setChannelVolume(int id, int volume)
{
  AudioChannel *channel = channels.value(id);
  if (!channel)
  {
    qDebug() << "Tried to set volume on non-existant channel";
    return;
  }
  channel->setChannelVolume(volume);
}

void AudioBackend::pauseChannel(int id)
{
  AudioChannel *channel = channels.value(id);
  if (!channel)
  {
    qDebug() << "Tried to pause a non-existant channel";
    return;
  }
  channel->pause();
}

void AudioBackend::resumeChannel(int id)
{
  AudioChannel *channel = channels.value(id);
  if (!channel)
  {
    qDebug() << "Tried to resume a non-existant channel";
    return;
  }
  channel->start();
}
