#include "audio.h"
#include "audiobackend.h"

Audio::Audio(QObject *parent)
    : QObject{parent}
{}

void Audio::setBackend(AudioBackend *f_backend)
{
  if (backend)
  {
    backend->deleteLater();
  }
  backend = f_backend;
}

QList<QString> Audio::devices()
{
  if (backend)
  {
    return backend->availableDevices();
  }
  return QStringList();
}

QList<QString> Audio::songs()
{
  if (backend)
  {
    return backend->availableSongs();
  }
  return QStringList();
}

int Audio::volume(int channel)
{
  if (backend)
  {
    return backend->volume(channel);
  }
  return 0;
}

void Audio::setDevicePerChannel(int channel, QString device)
{}

void Audio::pauseChannel(int channel)
{
  if (backend)
  {
    backend->pauseChannel(channel);
  }
}

void Audio::resumeChannel(int channel)
{
  if (backend)
  {
    backend->resumeChannel(channel);
  }
}

void Audio::setChannelSong(int channel, QString song)
{
  if (backend)
  {
    backend->setChannelSong(channel, song);
  }
}

void Audio::setChannelVolume(int channel, int volume)
{
  if (backend)
  {
    backend->setChannelVolume(channel, volume);
  }
}
