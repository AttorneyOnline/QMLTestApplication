#include "audio.h"
#include "audiobackend.h"

#include <QDebug>
#include <QTimer>

Audio::Audio(QObject *parent)
    : QObject{parent}
{}

void Audio::setBackend(AudioBackend *backend)
{
  if (m_backend)
  {
    qDebug() << "Audio: Replacing existing backend";
    disconnect(m_backend, nullptr, nullptr, nullptr);
    m_backend->deleteLater();
  }

  m_backend = backend;
  updateAllOptions();
  connectSignals();
  qDebug() << "Audio: Backend set successfully";
}

bool Audio::fadeOut() const
{
  return fadeOutProp;
}

void Audio::setFadeOut(bool enabled)
{
  if (!m_backend)
  {
    qDebug() << "Audio: Failed to set fadeOut - Backend not installed";
    return;
  }

  if (fadeOutProp == enabled)
  {
    qDebug() << "Audio: fadeOut value unchanged:" << enabled;
    return;
  }

  fadeOutProp = enabled;
  m_backend->setFadeOutEnabled(enabled);
  qDebug() << "Audio: fadeOut changed to" << enabled;
}

QBindable<bool> Audio::bindableFadeOut() const
{
  return QBindable<bool>(&fadeOutProp);
}

bool Audio::fadeIn() const
{
  return fadeInProp;
}

void Audio::setFadeIn(bool enabled)
{
  if (!m_backend)
  {
    qDebug() << "Audio: Failed to set fadeIn - Backend not installed";
    return;
  }

  if (fadeInProp == enabled)
  {
    qDebug() << "Audio: fadeIn value unchanged:" << enabled;
    return;
  }

  fadeInProp.setValue(enabled);
  m_backend->setFadeInEnabled(enabled);
  qDebug() << "Audio: fadeIn changed to" << enabled;
}

QBindable<bool> Audio::bindableFadeIn() const
{
  return QBindable<bool>(&fadeInProp);
}

bool Audio::synchronize() const
{
  return synchronizeProp;
}

void Audio::setSynchronize(bool enabled)
{
  if (!m_backend)
  {
    qDebug() << "Audio: Failed to set synchronize - Backend not installed";
    return;
  }

  if (synchronizeProp == enabled)
  {
    qDebug() << "Audio: synchronize value unchanged:" << enabled;
    return;
  }

  synchronizeProp = enabled;
  m_backend->setSynchronizeEnabled(enabled);
  qDebug() << "Audio: synchronize changed to" << enabled;
}

QBindable<bool> Audio::bindableSynchronize() const
{
  return QBindable<bool>(&synchronizeProp);
}

QStringList Audio::musiclist() const
{
  return musiclistProp;
}

void Audio::setMusiclist(const QStringList &musiclist)
{
  if (!m_backend)
  {
    qDebug() << "Audio: Failed to set musiclist - Backend not installed";
    return;
  }

  if (musiclistProp.value() == musiclist)
  {
    qDebug() << "Audio: musiclist value unchanged with" << musiclist.size() << "items";
    return;
  }

  musiclistProp = musiclist;
  qDebug() << "Audio: musiclist updated with" << musiclist.size() << "items";
}

QBindable<QStringList> Audio::bindableMusiclist() const
{
  return QBindable<QStringList>(&musiclistProp);
}

QStringList Audio::audioDevices() const
{
  return audioDevicesProp;
}

void Audio::setAudioDevices(const QStringList &audioDevices)
{
  if (!m_backend)
  {
    qDebug() << "Audio: Failed to set audioDevices - Backend not installed";
    return;
  }

  if (audioDevicesProp.value() == audioDevices)
  {
    return; // No change needed
  }

  audioDevicesProp = audioDevices;
  qDebug() << "Audio: audioDevices updated with" << audioDevices.size() << "devices:" << audioDevices;
}

QBindable<QStringList> Audio::bindableAudioDevices() const
{
  return QBindable<QStringList>(&audioDevicesProp);
}

// ---------- VOLUME BINDINGS ----------

int Audio::channel0Volume() const
{
  return channel0VolumeProp;
}

void Audio::setChannel0Volume(int volume)
{
  channel0VolumeProp = volume;
  if (!m_backend)
  {
    qDebug() << "Audio: Failed to set channel volume - Backend not installed";
    return;
  }
  m_backend->setVolume(volume, 0);
}

QBindable<int> Audio::bindableChannel0Volume() const
{
  return &channel0VolumeProp;
}

int Audio::channel1Volume() const
{
  return channel1VolumeProp;
}

void Audio::setChannel1Volume(int volume)
{
  channel1VolumeProp = volume;
  if (!m_backend)
  {
    qDebug() << "Audio: Failed to set channel volume - Backend not installed";
    return;
  }
  m_backend->setVolume(volume, 1);
}

QBindable<int> Audio::bindableChannel1Volume() const
{
  return &channel1VolumeProp;
}

int Audio::channel2Volume() const
{
  return channel2VolumeProp;
}

void Audio::setChannel2Volume(int volume)
{
  channel2VolumeProp = volume;
  if (!m_backend)
  {
    qDebug() << "Audio: Failed to set channel volume - Backend not installed";
    return;
  }
  m_backend->setVolume(volume, 2);
}

QBindable<int> Audio::bindableChannel2Volume() const
{
  return &channel2VolumeProp;
}

int Audio::channel3Volume() const
{
  return channel3VolumeProp;
}

void Audio::setChannel3Volume(int volume)
{
  channel3VolumeProp = volume;
  if (!m_backend)
  {
    qDebug() << "Audio: Failed to set channel volume - Backend not installed";
    return;
  }
  m_backend->setVolume(volume, 3);
}

QBindable<int> Audio::bindableChannel3Volume() const
{
  return &channel3VolumeProp;
}

// ---------- DEVICE Bindings ----------

QString Audio::channel0Device() const
{
  return channel0DeviceProp;
}

void Audio::setChannel0Device(const QString &device)
{
  channel0DeviceProp = device;
  if (!m_backend)
  {
    qDebug() << "Audio: Failed to set channel device - Backend not installed";
    return;
  }
  m_backend->setDevice(device, 0);
}

QBindable<QString> Audio::bindableChannel0Device() const
{
  return &channel0DeviceProp;
}

QString Audio::channel1Device() const
{
  return channel1DeviceProp;
}

void Audio::setChannel1Device(const QString &device)
{
  channel1DeviceProp = device;
  if (!m_backend)
  {
    qDebug() << "Audio: Failed to set channel device - Backend not installed";
    return;
  }
  m_backend->setDevice(device, 1);
}

QBindable<QString> Audio::bindableChannel1Device() const
{
  return &channel1DeviceProp;
}

QString Audio::channel2Device() const
{
  return channel2DeviceProp;
}

void Audio::setChannel2Device(const QString &device)
{
  channel2DeviceProp = device;
  if (!m_backend)
  {
    qDebug() << "Audio: Failed to set channel device - Backend not installed";
    return;
  }
  m_backend->setDevice(device, 2);
}

QBindable<QString> Audio::bindableChannel2Device() const
{
  return &channel2DeviceProp;
}

QString Audio::channel3Device() const
{
  return channel3DeviceProp;
}

void Audio::setChannel3Device(const QString &device)
{
  channel3DeviceProp = device;
  if (!m_backend)
  {
    qDebug() << "Audio: Failed to set channel device - Backend not installed";
    return;
  }
  m_backend->setDevice(device, 3);
}

QBindable<QString> Audio::bindableChannel3Device() const
{
  return &channel3DeviceProp;
}

void Audio::setVolume(int volume, int channel_id)
{
  switch (channel_id)
  {
  case 0:
    setChannel0Volume(volume);
    break;
  case 1:
    setChannel1Volume(volume);
    break;
  case 2:
    setChannel2Volume(volume);
    break;
  case 3:
    setChannel3Volume(volume);
    break;
  default:
    qWarning() << "Invalid channel_id:" << channel_id;
    break;
  }
}

void Audio::setDevice(const QString &device, int channel_id)
{
  switch (channel_id)
  {
  case 0:
    setChannel0Device(device);
    break;
  case 1:
    setChannel1Device(device);
    break;
  case 2:
    setChannel2Device(device);
    break;
  case 3:
    setChannel3Device(device);
    break;
  default:
    qWarning() << "Invalid channel_id:" << channel_id;
    break;
  }
}

void Audio::playSong(const QString &song, int channel_id)
{
  if (!m_backend)
  {
    qDebug() << "Audio: Failed to play song - Backend not installed";
    return;
  }
  m_backend->play(song, channel_id);
}

void Audio::resumeSong(int channel_id)
{
  if (!m_backend)
  {
    qDebug() << "Audio: Failed to resume song - Backend not installed";
    return;
  }
  m_backend->resume(channel_id);
}

void Audio::pauseSong(int channel_id)
{
  if (!m_backend)
  {
    qDebug() << "Audio: Failed to pause song - Backend not installed";
    return;
  }
  m_backend->pause(channel_id);
}

void Audio::stopSong(int channel_id)
{
  if (!m_backend)
  {
    qDebug() << "Audio: Failed to stop song - Backend not installed";
    return;
  }
  m_backend->stop(channel_id);
}

void Audio::musicListUpdated(const QStringList &musiclist)
{
  setMusiclist(musiclist);
}

void Audio::audioDevicesUpdated(const QStringList &audioDevices)
{
  setAudioDevices(audioDevices);
}

void Audio::updateAllOptions()
{
  if (!m_backend)
  {
    qDebug() << "Audio: Cannot update options - Backend not installed";
    return;
  }

  qDebug() << "Audio: Updating all options from backend";
  setFadeIn(m_backend->fadeInEnabled());
  setFadeOut(m_backend->fadeOutEnabled());
  setSynchronize(m_backend->synchronizeEnabled());
  setMusiclist(m_backend->songs());
  setAudioDevices(m_backend->devices());

  setChannel0Volume(m_backend->volume(0));
  setChannel1Volume(m_backend->volume(1));
  setChannel2Volume(m_backend->volume(2));
  setChannel3Volume(m_backend->volume(3));

  setChannel0Device(m_backend->device(0));
  setChannel1Device(m_backend->device(1));
  setChannel2Device(m_backend->device(2));
  setChannel3Device(m_backend->device(3));
}

void Audio::connectSignals()
{
  connect(m_backend, &AudioBackend::songsUpdated, this, &Audio::setMusiclist);
  connect(m_backend, &AudioBackend::devicesUpdated, this, &Audio::audioDevicesUpdated);
}
