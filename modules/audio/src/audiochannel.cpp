#include "audiochannel.h"
#include "audioerror.h"

#include <QDebug>

namespace
{
  void CALLBACK downloadproc(const void *buffer, DWORD length, void *user)
  {
    Q_UNUSED(buffer)
    Q_UNUSED(user)
    qDebug() << "AudioChannel: Downloading song from remote source. Bytes in buffer" << length;
  };

  void CALLBACK endSyncProc(HSYNC handle, DWORD channel, DWORD data, void *user)
  {
    Q_UNUSED(handle)
    Q_UNUSED(data);

    QWORD loop_start = *static_cast<QWORD *>(user);
    BASS_ChannelLock(channel, true);
    BASS_ChannelSetPosition(channel, loop_start, BASS_POS_BYTE);
    BASS_ChannelLock(channel, false);
  }

  void CALLBACK deviceLostProc(HSYNC handle, DWORD channel, DWORD data, void *user)
  {
    Q_UNUSED(handle);
    Q_UNUSED(data);
    Q_UNUSED(user);

    // Reset to the default device.
    if (!BASS_ChannelSetDevice(channel, -1))
    {
      qDebug() << "AudioChannel: Failed to set audio device: " << AudioError::getErrorMessage();
    }
  }

  void CALLBACK fadeOutCompleteProc(HSYNC handle, DWORD channel, DWORD data, void *user)
  {
    Q_UNUSED(handle);
    Q_UNUSED(data);
    Q_UNUSED(user);

    if (channel)
    {
      qDebug() << "AudioChannel: Fade out complete, stopping stream.";
      BASS_ChannelStop(channel);
    }
  }
} // namespace

AudioChannel::AudioChannel(QObject *parent)
    : QObject(parent)
    , audio_enabled{true}
{
  qDebug() << "AudioChannel: Created new audiochannel.";
}

AudioChannel::~AudioChannel()
{
  if (!BASS_ChannelStop(stream))
  {
    qDebug() << "AudioChannel: Failed to stop BASS stream:" << AudioError::getErrorMessage();
  }
}

void AudioChannel::setSong(const QString &file_path)
{
  if (file_path.isEmpty())
  {
    qDebug() << "AudioChannel: Failed to set song: Provided path is empty.";
    return;
  }

  DWORD flags = BASS_STREAM_AUTOFREE | BASS_SAMPLE_LOOP | BASS_STREAM_PRESCAN;
  if (file_path.startsWith("http://") || file_path.startsWith("https://"))
  {
    stream = BASS_StreamCreateURL(file_path.toUtf8().constData(), 0, flags, &downloadproc, nullptr);
  }

  else
  {
    flags |= BASS_UNICODE | BASS_ASYNCFILE;
    stream = BASS_StreamCreateFile(false, file_path.utf16(), 0, 0, flags);
  }

  if (!stream)
  {
    qDebug() << "AudioChannel: Failed to create stream for file " << file_path << ":" << AudioError::getErrorMessage();
    return;
  }

  if (!BASS_ChannelSetSync(stream, BASS_SYNC_DEV_FAIL, BASS_SYNC_MIXTIME, deviceLostProc, nullptr))
  {
    qDebug() << "AudioChannel: Failed to set device lost callback: " << AudioError::getErrorMessage();
  }

  qDebug() << "AudioChannel: Loaded audio file " << file_path;
}

void AudioChannel::setDevice(DWORD device_id)
{
  if (!stream)
  {
    qDebug() << "AudioChannel: Failed to set device. Create a stream using playSong() first.";
    return;
  }

  if (!BASS_ChannelSetDevice(stream, device_id))
  {
    qDebug() << "AudioChannel: Failed to set audio device: " << AudioError::getErrorMessage();
  }
}

void AudioChannel::setVolume(int target_volume)
{
  int clamped_volume = std::clamp(target_volume, 0, 100);
  audio_volume = clamped_volume;

  if (!stream)
  {
    qDebug() << "AudioChannel: Failed to set volume. Create a stream using playSong() first.";
    return;
  }

  if (!BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, calculateVolume()))
  {
    qDebug() << "AudioChannel: Failed to set volume: " << AudioError::getErrorMessage();
  }
}

void AudioChannel::setLoopPoints(double start, double end)
{
  if (!stream)
  {
    qDebug() << "AudioChannel: No stream available. Please use setSong() before setting loop points.";
    return;
  }

  QWORD loop_start = BASS_ChannelSeconds2Bytes(stream, start);
  QWORD loop_end = BASS_ChannelSeconds2Bytes(stream, end);

  const QWORD length = BASS_ChannelGetLength(stream, BASS_POS_BYTE);
  qDebug() << "AudioChannel: Loop points determined - Start: " << loop_start << " - End: " << loop_end << " - Length: " << length;

  const QWORD sync_pos = (loop_start < loop_end) ? loop_end : 0;

  if (sync_pos)
  {
    if (!BASS_ChannelSetSync(stream, BASS_SYNC_POS | BASS_SYNC_MIXTIME, sync_pos, endSyncProc, &loop_start))
    {
      qDebug() << "AudioChannel: Failed to setup loop: " << AudioError::getErrorMessage();
    }
  }
  else
  {
    if (!BASS_ChannelSetSync(stream, BASS_SYNC_END | BASS_SYNC_MIXTIME, 0, endSyncProc, &loop_start))
    {
      qDebug() << "AudioChannel: Failed to setup loop: " << AudioError::getErrorMessage();
    }
  }
}

void AudioChannel::setEnabled(bool state)
{
  audio_enabled = state;
  setVolume(audio_volume);
}

void AudioChannel::start()
{
  if (!BASS_ChannelPlay(stream, false))
  {
    qDebug() << "AudioChannel: Failed to start stream: " << AudioError::getErrorMessage();
    return;
  }
  qDebug() << "AudioChannel: Started playback.";
}

void AudioChannel::pause()
{
  if (!BASS_ChannelPause(stream))
  {
    qDebug() << "AudioChannel: Failed to pause stream: " << AudioError::getErrorMessage();
    return;
  }
  qDebug() << "AudioChannel: Paused playback";
}

void AudioChannel::stop()
{
  if (!BASS_ChannelStop(stream))
  {
    qDebug() << "AudioChannel: Failed to stop stream: " << AudioError::getErrorMessage();
    return;
  }
  qDebug() << "AudioChannel: Stopped playback. Stream has been invalidated.";
}

void AudioChannel::fadeOut(int duration)
{
  if (!stream)
  {
    qDebug() << "AudioChannel: Failed to fade out. No stream available.";
    return;
  }

  if (!BASS_ChannelSetSync(stream, BASS_SYNC_SLIDE, 0, fadeOutCompleteProc, nullptr))
  {
    qDebug() << "AudioChannel: Failed to set fade out completion callback: " << AudioError::getErrorMessage();
  }

  if (!BASS_ChannelSlideAttribute(stream, BASS_ATTRIB_VOL, 0.0f, duration))
  {
    qDebug() << "AudioChannel: Failed to start fade out: " << AudioError::getErrorMessage();
    return;
  }

  qDebug() << "AudioChannel: Started fade out over" << duration << "milliseconds";
}

void AudioChannel::fadeIn(int duration)
{
  if (!stream)
  {
    qDebug() << "AudioChannel: Failed to fade in. No stream available.";
    return;
  }

  if (!BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, 0.0f))
  {
    qDebug() << "AudioChannel: Failed to set initial fade in volume: " << AudioError::getErrorMessage();
    return;
  }

  if (BASS_ChannelIsActive(stream) != BASS_ACTIVE_PLAYING)
  {
    if (!BASS_ChannelPlay(stream, false))
    {
      qDebug() << "AudioChannel: Failed to start stream for fade in: " << AudioError::getErrorMessage();
      return;
    }
  }

  float target_volume = calculateVolume();
  if (!BASS_ChannelSlideAttribute(stream, BASS_ATTRIB_VOL, target_volume, duration))
  {
    qDebug() << "AudioChannel: Failed to start fade in: " << AudioError::getErrorMessage();
    return;
  }

  qDebug() << "AudioChannel: Started fade in over" << duration << "milliseconds to volume" << target_volume;
}

const QString AudioChannel::song()
{
  return m_song;
}

QWORD AudioChannel::position()
{
  return BASS_ChannelGetPosition(stream, BASS_POS_BYTE);
}

void AudioChannel::setPosition(QWORD byte_position)
{
  BASS_ChannelLock(stream, true);
  if (BASS_ChannelSetPosition(stream, byte_position, BASS_POS_BYTE))
  {
    qDebug() << "AudioChannel: Failed to set position at" << byte_position << ":" << AudioError::getErrorMessage();
  }
  BASS_ChannelLock(stream, false);
}

const float AudioChannel::calculateVolume()
{
  return (audio_volume / 100.0f) * (audio_enabled ? 1.0f : 0.0f);
}
