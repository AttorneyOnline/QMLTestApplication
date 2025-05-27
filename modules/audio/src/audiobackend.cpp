#include "audiobackend.h"
#include "audiochannel.h"
#include "audioerror.h"
#include "audiosettings.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>

namespace
{
  constexpr int DEFAULT_SAMPLE_RATE = 48000;
  constexpr int DEFAULT_VOLUME = 25;
  constexpr int BASS_NO_PLUGIN = 0;
  constexpr int MAX_CHANNELS = 4;
  constexpr int BASS_UPDATE_THREADS = 4;

  Q_GLOBAL_STATIC_WITH_ARGS(QString, musicDirectory, (QStringLiteral("./music")))
  Q_GLOBAL_STATIC_WITH_ARGS(QStringList, supportedFormats, (QStringList{"*.opus", "*.mp3", "*.wav"}))
} // namespace

AudioBackend::AudioBackend(QObject *parent)
    : QObject(parent)
    , m_settings(new AudioSettings(this))
{
  BASS_Free();
  BASS_SetConfig(BASS_CONFIG_DEV_DEFAULT, 1);
  BASS_SetConfig(BASS_CONFIG_UPDATETHREADS, BASS_UPDATE_THREADS);

  if (BASS_PluginLoad("bassopus", BASS_NO_PLUGIN))
  {
    qDebug() << "AudioBackend: BASSOPUS plugin loaded successfully";
  }
  else
  {
    qDebug() << "AudioBackend: Failed to load BASSOPUS plugin:" << AudioError::getErrorMessage();
  }

  for (int channel = 0; channel < MAX_CHANNELS; channel++)
  {
    m_channels[channel] = new AudioChannel(this);
  }

  loadDevices();
  loadSongs();
}

AudioBackend::~AudioBackend()
{
  qDeleteAll(m_channels);
  BASS_Free();
}

QStringList AudioBackend::songs() const
{
  return m_songs;
}

QStringList AudioBackend::devices() const
{
  return m_devices;
}

void AudioBackend::play(const QString &song, int channel)
{
  AudioChannel *ch = getChannel(channel);
  if (!ch)
    return;

  QWORD position = ch->position();

  if (fadeOutEnabled())
  {
    const int fadeout_duration = m_settings->fadeOutDuration(ch->song());
    ch->fadeOut(fadeout_duration);
  }
  else
  {
    ch->stop();
  }

  ch->setSong(song);
  ch->setDevice(m_devices.indexOf(m_settings->channelAudioDevice(channel)));
  ch->setVolume(m_settings->channelVolume(channel));

  QPair<double, double> loop_points = m_settings->loopPoints(song);
  ch->setLoopPoints(loop_points.first, loop_points.second);

  if (fadeInEnabled())
  {
    const int fadein_duration = m_settings->fadeInDuration(song);
    ch->fadeIn(fadein_duration);
  }
  else
  {
    ch->start();
  }

  if (synchronizeEnabled())
  {
    ch->setPosition(position);
  }
}

void AudioBackend::resume(int channel)
{
  AudioChannel *ch = getChannel(channel);
  if (!ch)
    return;
  ch->start();
}

void AudioBackend::pause(int channel)
{
  AudioChannel *ch = getChannel(channel);
  if (!ch)
    return;
  ch->pause();
}

void AudioBackend::stop(int channel)
{
  AudioChannel *ch = getChannel(channel);
  if (!ch)
    return;
  ch->stop();
}

int AudioBackend::volume(int channel_id)
{
  return m_settings->channelVolume(channel_id);
}

void AudioBackend::setVolume(int volume, int channel)
{
  AudioChannel *ch = getChannel(channel);
  if (!ch)
    return;

  m_settings->setChannelVolume(channel, volume);
  ch->setVolume(volume);
}

QString AudioBackend::device(int channel_id)
{
  return m_settings->channelAudioDevice(channel_id);
}

void AudioBackend::setDevice(const QString &device, int channel)
{
  AudioChannel *ch = getChannel(channel);
  if (!ch)
    return;

  m_settings->setChannelAudioDevice(channel, device);

  const QWORD device_id = m_devices.indexOf(device);
  ch->setDevice(device_id);
}

void AudioBackend::setFadeInEnabled(bool enabled)
{
  m_settings->setFadeInEnabled(enabled);
}

void AudioBackend::setFadeOutEnabled(bool enabled)
{
  m_settings->setFadeOutEnabled(enabled);
}

void AudioBackend::setSynchronizeEnabled(bool enabled)
{
  m_settings->setSynchronizedEnabled(enabled);
}

bool AudioBackend::fadeInEnabled()
{
  return m_settings->fadeInEnabled();
}

bool AudioBackend::fadeOutEnabled()
{
  return m_settings->fadeOutEnabled();
}

bool AudioBackend::synchronizeEnabled()
{
  return m_settings->synchronizeEnabled();
}

void AudioBackend::loadDevices()
{
  m_devices.clear();
  BASS_DEVICEINFO device_info;

  for (int device_id = 0; BASS_GetDeviceInfo(device_id, &device_info); ++device_id)
  {
    QString device_name = QString::fromUtf8(device_info.name);

    if (BASS_Init(device_id, DEFAULT_SAMPLE_RATE, BASS_DEVICE_LATENCY, nullptr, nullptr))
    {
      m_devices.append(device_name);
      qDebug() << "AudioBackend: Initialized audio device:" << device_name;
    }
    else
    {
      qDebug() << "AudioBackend: Failed to initialize device:" << device_name << "-" << AudioError::getErrorMessage();
    }
  }

  if (m_devices.isEmpty())
  {
    qDebug() << "AudioBackend: Warning: No audio devices could be initialized";
  }
  else
  {
    qDebug() << "AudioBackend: Initialized" << m_devices.size() << "audio devices";
  }
  Q_EMIT devicesUpdated(m_devices);
}

void AudioBackend::loadSongs()
{
  m_songs.clear();
  QDir musicDir(*musicDirectory);
  QDir projectDir;

  QDirIterator it(musicDir.absolutePath(), *supportedFormats, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
  while (it.hasNext())
  {
    it.next();
    QString relativePath = projectDir.relativeFilePath(it.filePath());
    m_songs.append(relativePath);
  }
  Q_EMIT songsUpdated(m_songs);
}

AudioChannel *AudioBackend::getChannel(int channel)
{
  return m_channels.value(channel, nullptr);
}
