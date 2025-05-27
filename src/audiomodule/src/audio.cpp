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

void Audio::setChannelVolume(int channel, int volume)
{
  if (backend)
  {
    backend->setChannelVolume(channel, volume);
  }
}
