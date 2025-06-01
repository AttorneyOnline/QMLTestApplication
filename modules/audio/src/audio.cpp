#include "audio.h"
#include "audiobackend.h"

Audio::Audio(QObject *parent)
    : QObject{parent}
{}

void Audio::setBackend(AudioBackend *f_backend)
{
  if (m_backend)
  {
    m_backend->deleteLater();
  }
  m_backend = f_backend;
}

QList<QString> Audio::availableDevices() const
{
  if (m_backend)
  {
    return m_backend->availableDevices();
  }
  return QStringList();
}

QList<QString> Audio::availableSongs() const
{
  if (m_backend)
  {
    return m_backend->availableSongs();
  }
  return QStringList();
}

int Audio::volume(int channel)
{
  if (m_backend)
  {
    return m_backend->volume(channel);
  }
  return 0;
}

QString Audio::device(int channel_id)
{
  if (m_backend)
  {
    return m_backend->device(channel_id);
  }
  return QString{};
}

bool Audio::fadeOut()
{
  if (m_backend)
  {
    return m_backend->fadeOut();
  }
  return false;
}

bool Audio::fadeIn()
{
  if (m_backend)
  {
    return m_backend->fadeIn();
  }
  return false;
}

void Audio::stop(int channel_id)
{
  if (m_backend)
  {
    m_backend->stopChannel(channel_id);
  }
}

void Audio::setDevice(int channel_id, const QString &device)
{
  if (m_backend)
  {
    m_backend->setChannelDevice(channel_id, device);
  }
}

void Audio::pause(int channel)
{
  if (m_backend)
  {
    m_backend->pauseChannel(channel);
  }
}

void Audio::resume(int channel)
{
  if (m_backend)
  {
    m_backend->resumeChannel(channel);
  }
}

void Audio::setSong(int channel, const QString &song)
{
  if (m_backend)
  {
    m_backend->setChannelSong(channel, song);
  }
}

void Audio::setVolume(int channel, int volume)
{
  if (m_backend)
  {
    m_backend->setChannelVolume(channel, volume);
  }
}

void Audio::setFadeOut(bool state)
{
  if (m_backend)
  {
    m_backend->setFadeOut(state);
  }
}

void Audio::setFadeIn(bool state)
{
  if (m_backend)
  {
    m_backend->setFadeIn(state);
  }
}
