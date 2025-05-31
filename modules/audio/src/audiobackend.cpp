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
  // Clean up channels
  for (AudioChannel *channel : std::as_const(m_channels))
  {
    if (channel)
    {
      channel->deleteLater();
    }
  }
  m_channels.clear();

  BASS_Free();
}

// Audio file management
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
  QString device_name = m_settings->channelAudioDevice(channel_id);
  int device_id = m_audio_devices.value(device_name, 0);

  AudioChannel *new_channel = new AudioChannel(channel_id, device_id, this);
  new_channel->setFile(song_path);
  new_channel->setVolume(m_settings->channelVolume(device_id));

  replaceChannel(channel_id, new_channel);
  new_channel->start();

  qDebug() << "Set song" << song_path << "on channel" << channel_id;
}

void AudioBackend::setChannelVolume(int channel_id, int volume)
{
  AudioChannel *channel = getChannel(channel_id);
  if (!channel)
  {
    qDebug() << "Cannot set volume: channel" << channel_id << "does not exist";
    return;
  }

  m_settings->setChannelVolume(channel_id, volume);
  channel->setVolume(volume);
}

void AudioBackend::pauseChannel(int channel_id)
{
  AudioChannel *channel = getChannel(channel_id);
  if (!channel)
  {
    qDebug() << "Cannot pause: channel" << channel_id << "does not exist";
    return;
  }

  channel->pause();
}

void AudioBackend::resumeChannel(int channel_id)
{
  AudioChannel *channel = getChannel(channel_id);
  if (!channel)
  {
    qDebug() << "Cannot resume: channel" << channel_id << "does not exist";
    return;
  }

  channel->start();
}

int AudioBackend::volume(int channel_id)
{
  return m_settings->channelVolume(channel_id);
}

AudioChannel *AudioBackend::getChannel(int channel_id) const
{
  return m_channels.value(channel_id, nullptr);
}

void AudioBackend::replaceChannel(int channel_id, AudioChannel *new_channel)
{
  AudioChannel *old_channel = m_channels.value(channel_id);
  if (old_channel)
  {
    old_channel->deleteLater();
  }
  m_channels.insert(channel_id, new_channel);
}
