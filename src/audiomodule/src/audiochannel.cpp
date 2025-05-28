#include "audiochannel.h"
#include "audioerror.h"

#include <QDebug>

AudioChannel::AudioChannel(int channel_id, int device_id, QObject *parent)
    : QObject{parent}
    , channel{channel_id}
    , device{device_id}
    , audio_enabled{true}
{}

AudioChannel::~AudioChannel()
{
  if (!BASS_ChannelStop(stream))
  {
    qDebug() << "Failed to stop bass Channel:" << AudioError::getErrorMessage();
  }
}

void CALLBACK endSyncProc(HSYNC handle, DWORD channel, DWORD data, void *user)
{
  qDebug() << "Loop Callback reached";

  Q_UNUSED(handle)
  Q_UNUSED(data)
  QWORD loop_start = *(static_cast<unsigned *>(user));
  BASS_ChannelLock(channel, true);
  BASS_ChannelSetPosition(channel, loop_start, BASS_POS_BYTE);
  BASS_ChannelLock(channel, false);
}

void CALLBACK deviceLost()
{
  qDebug() << "BASS AUDIO DEVICE LOST";
}

void AudioChannel::setFile(QString f_file)
{
  DWORD flags = BASS_STREAM_AUTOFREE | BASS_SAMPLE_LOOP; // Automatic Cleanup, always looping

  if (f_file.startsWith("http"))
  {
    stream = BASS_StreamCreateURL(f_file.toUtf8(), 0, flags, nullptr, nullptr);
  }
  else
  {
    flags |= BASS_STREAM_PRESCAN | BASS_ASYNCFILE; // accurate seek points and length reading, asynchronous reading/decoding
    stream = BASS_StreamCreateFile(false, f_file.toUtf8(), 0, 0, flags);
  }

  if (!stream)
  {
    qDebug() << "Failed to create stream:" << AudioError::getErrorMessage();
  }
  setChannelDevice(device);
}

void AudioChannel::setLoopPoints(double start, double end)
{
  loop_start = BASS_ChannelSeconds2Bytes(stream, start);
  loop_end = BASS_ChannelSeconds2Bytes(stream, end);

  qDebug() << "Length:" << BASS_ChannelGetLength(stream, BASS_POS_BYTE) << "loop_start" << loop_start << "loop_end" << loop_end;

  if (loop_start < loop_end)
  {
    BASS_ChannelSetSync(stream, BASS_SYNC_POS | BASS_SYNC_MIXTIME, loop_end, endSyncProc, &loop_start);
  }
  else
  {
    BASS_ChannelSetSync(stream, BASS_SYNC_POS | BASS_SYNC_MIXTIME, 0, endSyncProc, &loop_start);
  }
}

void AudioChannel::setChannelVolume(int volume)
{
  float f_volume = (std::clamp(volume, 0, 100) / 100.0f) * audio_enabled;
  BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, f_volume);
}

void AudioChannel::setChannelDevice(DWORD device_id)
{
  if (!BASS_ChannelSetDevice(stream, device_id))
  {
    qDebug() << "Unable to update audio device on channel:" << AudioError::getErrorMessage();
  }
}

void AudioChannel::start()
{
  if (!BASS_ChannelStart(stream))
  {
    qDebug() << "Failed to start stream:" << AudioError::getErrorMessage();
  }
}

void AudioChannel::stop()
{
  if (BASS_ChannelPause(stream))
  {
    qDebug() << "Failed to pause stream:" << AudioError::getErrorMessage();
  }
}

void AudioChannel::setAudioEnabled(bool enabled)
{
  audio_enabled = enabled;
  setChannelVolume(volume);
}
