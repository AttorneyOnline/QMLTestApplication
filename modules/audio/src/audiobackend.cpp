#include "audiobackend.h"
#include "audiochannel.h"
#include "audioerror.h"
#include "audiosettings.h"

#include <QDebug>
#include <QDir>

namespace
{
  constexpr int DEFAULT_SAMPLE_RATE = 48000;
  constexpr int DEFAULT_VOLUME = 25;
  constexpr int BASS_NO_PLUGIN = 0;

  Q_GLOBAL_STATIC_WITH_ARGS(QString, musicDirectory, (QStringLiteral("./music")))
  Q_GLOBAL_STATIC_WITH_ARGS(QStringList, supportedFormats, (QStringList{"*.opus", "*.mp3", "*.wav"}))
} // namespace

AudioBackend::AudioBackend(QObject *parent)
    : QObject(parent)
    , m_settings(new AudioSettings(this))
{
  BASS_Free();
  BASS_SetConfig(BASS_CONFIG_DEV_DEFAULT, 1);

  if (BASS_PluginLoad("bassopus", BASS_NO_PLUGIN))
  {
    qDebug() << "BASSOPUS plugin loaded successfully";
  }
  else
  {
    qDebug() << "Failed to load BASSOPUS plugin:" << AudioError::getErrorMessage();
  }

  initializeAudioDevices();
}

AudioBackend::~AudioBackend()
{
  qDeleteAll(m_channels);
  BASS_Free();
}

QStringList AudioBackend::availableSongs() const
{
  QDir music_dir(*musicDirectory());
  music_dir.setNameFilters(*supportedFormats());
  music_dir.setFilter(QDir::NoDotAndDotDot | QDir::Files);

  QStringList songs = music_dir.entryList();
  if (songs.isEmpty())
  {
    qDebug() << "No supported audio files found in" << *musicDirectory();
  }

  return songs;
}

QStringList AudioBackend::availableDevices() const
{
  return m_audio_devices.keys();
}

QString AudioBackend::device(int channel_id) const
{
  return m_settings->channelAudioDevice(channel_id);
}

void AudioBackend::initializeAudioDevices()
{
  BASS_DEVICEINFO device_info;

  for (int device_id = 0; BASS_GetDeviceInfo(device_id, &device_info); ++device_id)
  {
    QString device_name = QString::fromUtf8(device_info.name);

    if (BASS_Init(device_id, DEFAULT_SAMPLE_RATE, BASS_DEVICE_LATENCY, nullptr, nullptr))
    {
      m_audio_devices.insert(device_name, device_id);
      qDebug() << "Initialized audio device:" << device_name;
    }
    else
    {
      qDebug() << "Failed to initialize device:" << device_name << "-" << AudioError::getErrorMessage();
    }
  }

  if (m_audio_devices.isEmpty())
  {
    qDebug() << "Warning: No audio devices could be initialized";
  }
  else
  {
    qDebug() << "Initialized" << m_audio_devices.size() << "audio devices";
  }
}

void AudioBackend::setChannelSong(int channel_id, const QString &song_path)
{
  if (song_path.isEmpty())
  {
    qDebug() << "Empty song path provided for channel" << channel_id;
    return;
  }

  // Device names are more consistent than device Ids. Thanks Windows.
  int device_id = m_audio_devices.value(m_settings->channelAudioDevice(channel_id));

  AudioChannel *new_channel = new AudioChannel(channel_id, device_id, this);
  new_channel->setFile(song_path);
  new_channel->setVolume(m_settings->channelVolume(channel_id));

  replaceChannel(channel_id, new_channel);
  new_channel->setLoopPoints(m_settings->loopPoints(song_path));
  new_channel->fadeIn(m_settings->fadeInDuration(song_path));
  new_channel->start();

  qDebug() << "Set song" << song_path << "on channel" << channel_id;
}

void AudioBackend::setChannelVolume(int channel_id, int volume)
{
  m_settings->setChannelVolume(channel_id, volume);

  if (AudioChannel *channel = getChannel(channel_id))
  {
    channel->setVolume(volume);
  }
  else
  {
    qDebug() << "Cannot set volume: channel" << channel_id << "does not exist";
  }
}

void AudioBackend::setChannelDevice(int channel_id, const QString &device_name)
{
  m_settings->setChannelAudioDevice(channel_id, device_name);

  if (AudioChannel *channel = getChannel(channel_id))
  {
    channel->setDevice(m_audio_devices.value(device_name));
  }
  else
  {
    qDebug() << "Cannot change device: channel" << channel_id << "does not exist";
  }
}

void AudioBackend::pauseChannel(int channel_id)
{
  if (AudioChannel *channel = getChannel(channel_id))
  {
    channel->pause();
  }
  else
  {
    qDebug() << "Cannot pause: channel" << channel_id << "does not exist";
  }
}

void AudioBackend::resumeChannel(int channel_id)
{
  if (AudioChannel *channel = getChannel(channel_id))
  {
    channel->start();
  }
  else
  {
    qDebug() << "Cannot resume: channel" << channel_id << "does not exist";
  }
}

void AudioBackend::stopChannel(int channel_id)
{
  if (AudioChannel *channel = getChannel(channel_id))
  {
    channel->stop();
  }
  else
  {
    qDebug() << "Cannot stop: channel" << channel_id << "does not exist";
  }
}

void AudioBackend::setFadeOut(bool state)
{
  m_settings->setFadeOutEnabled(state);
}

void AudioBackend::setFadeIn(bool state)
{
  m_settings->setFadeInEnabled(state);
}

int AudioBackend::volume(int channel_id) const
{
  return m_settings->channelVolume(channel_id);
}

bool AudioBackend::fadeOut() const
{
  return m_settings->fadeOutEnabled();
}

bool AudioBackend::fadeIn() const
{
  return m_settings->fadeInEnabled();
}

AudioChannel *AudioBackend::getChannel(int channel_id) const
{
  return m_channels.value(channel_id);
}

void AudioBackend::replaceChannel(int channel_id, AudioChannel *new_channel)
{
  if (AudioChannel *old_channel = m_channels.value(channel_id))
  {
    if (m_settings->fadeOutEnabled())
    {
      old_channel->fadeOut(m_settings->fadeOutDuration(old_channel->song()));
    }
    else
    {
      old_channel->deleteLater();
    }
  }
  m_channels.insert(channel_id, new_channel);
}
