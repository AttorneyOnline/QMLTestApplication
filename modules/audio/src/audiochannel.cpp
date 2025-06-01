#include "audiochannel.h"
#include "audioerror.h"

#include <QDebug>
#include <QTimer>
#include <algorithm>

// BASS callback functions
namespace
{
  void CALLBACK endSyncProc(HSYNC handle, DWORD channel, DWORD data, void *user)
  {
    Q_UNUSED(handle)
    Q_UNUSED(data)

    QWORD loop_start = *static_cast<QWORD *>(user);
    BASS_ChannelLock(channel, true);
    BASS_ChannelSetPosition(channel, loop_start, BASS_POS_BYTE);
    BASS_ChannelLock(channel, false);

    qDebug() << "Audio loop callback executed";
  }

  void CALLBACK deviceLostProc(void *)
  {
    qDebug() << "BASS audio device lost";
  }
} // namespace

AudioChannel::AudioChannel(int channel_id, int device_id, QObject *parent)
    : QObject(parent)
    , stream(0)
    , loop_start(0)
    , loop_end(0)
    , volume(100)
    , channel(channel_id)
    , device(device_id)
    , audio_enabled(true)
{}

AudioChannel::~AudioChannel()
{
  if (stream && !BASS_ChannelStop(stream))
  {
    qDebug() << "Failed to stop BASS channel:" << AudioError::getErrorMessage();
  }
}

void AudioChannel::setFile(const QString &file_path)
{
  if (file_path.isEmpty())
  {
    qDebug() << "Empty file path provided";
    return;
  }

  m_song = file_path;

  DWORD flags = BASS_STREAM_AUTOFREE | BASS_SAMPLE_LOOP;

  if (file_path.startsWith("http"))
  {
    stream = BASS_StreamCreateURL(file_path.toUtf8().constData(), 0, flags, nullptr, nullptr);
  }
  else
  {
    flags |= BASS_STREAM_PRESCAN | BASS_UNICODE | BASS_ASYNCFILE;
    stream = BASS_StreamCreateFile(false, file_path.utf16(), 0, 0, flags);
  }

  if (!stream)
  {
    qDebug() << "Failed to create stream for" << file_path << ":" << AudioError::getErrorMessage();
    return;
  }

  setDevice(device);
  qDebug() << "Audio file loaded:" << file_path;
}

void AudioChannel::start()
{
  if (!stream)
  {
    qDebug() << "No stream available to start";
    return;
  }

  if (!BASS_ChannelStart(stream))
  {
    qDebug() << "Failed to start stream:" << AudioError::getErrorMessage();
  }
}

void AudioChannel::pause()
{
  if (!stream)
  {
    qDebug() << "No stream available to pause";
    return;
  }

  if (!BASS_ChannelPause(stream))
  {
    qDebug() << "Failed to pause stream:" << AudioError::getErrorMessage();
  }
}

void AudioChannel::stop()
{
  if (!stream)
  {
    qDebug() << "No stream available to pause";
    return;
  }

  if (!BASS_ChannelStop(stream))
  {
    qDebug() << "Failed to stop stream:" << AudioError::getErrorMessage();
  }
}

void AudioChannel::fadeOut(int duration)
{
  if (!stream)
  {
    qDebug() << "No stream available for fade out";
    return;
  }

  float current_volume;
  if (!BASS_ChannelGetAttribute(stream, BASS_ATTRIB_VOL, &current_volume))
  {
    qDebug() << "Failed to get current volume for fade out:" << AudioError::getErrorMessage();
    return;
  }

  if (!BASS_ChannelSlideAttribute(stream, BASS_ATTRIB_VOL | BASS_SLIDE_LOG, -1, duration))
  {
    qDebug() << "Failed to start fade out:" << AudioError::getErrorMessage();
  }

  QTimer *kill = new QTimer(this);
  kill->setInterval(duration);
  kill->start();
  kill->callOnTimeout([this]() { deleteLater(); }); // Find a better way for this, good enough for now ...
}

void AudioChannel::fadeIn(int duration)
{
  if (!stream)
  {
    qDebug() << "No stream available for fade in";
    return;
  }

  float target_volume = calculateTargetVolume();

  BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, 0.0f);

  if (!BASS_ChannelSlideAttribute(stream, BASS_ATTRIB_VOL, target_volume, duration))
  {
    qDebug() << "Failed to start fade in:" << AudioError::getErrorMessage();
  }
}

void AudioChannel::setLoopPoints(const QPair<double, double> &points)
{
  if (!stream)
  {
    qDebug() << "No stream available to set loop points";
    return;
  }

  loop_start = BASS_ChannelSeconds2Bytes(stream, points.first);
  loop_end = BASS_ChannelSeconds2Bytes(stream, points.second);

  QWORD stream_length = BASS_ChannelGetLength(stream, BASS_POS_BYTE);
  qDebug() << "Stream length:" << stream_length << "Loop points:" << loop_start << "-" << loop_end;

  QWORD sync_position = (loop_start < loop_end) ? loop_end : 0;

  if (!BASS_ChannelSetSync(stream, BASS_SYNC_POS | BASS_SYNC_MIXTIME, sync_position, endSyncProc, &loop_start))
  {
    qDebug() << "Failed to set loop sync:" << AudioError::getErrorMessage();
  }
}

void AudioChannel::setVolume(int new_volume)
{
  volume = std::clamp(new_volume, 0, 100);

  if (!stream)
  {
    return;
  }

  float target_volume = calculateTargetVolume();

  if (!BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, target_volume))
  {
    qDebug() << "Failed to set volume:" << AudioError::getErrorMessage();
  }
}

void AudioChannel::setDevice(DWORD device_id)
{
  device = device_id;

  if (!stream)
  {
    return;
  }

  if (!BASS_ChannelSetDevice(stream, device_id))
  {
    qDebug() << "Failed to set audio device:" << AudioError::getErrorMessage();
  }
}

void AudioChannel::setEnabled(bool enabled)
{
  audio_enabled = enabled;
  setVolume(volume);
}

QString AudioChannel::song()
{
  return m_song;
}

float AudioChannel::calculateTargetVolume() const
{
  return (volume / 100.0f) * (audio_enabled ? 1.0f : 0.0f);
}
