#include "audiochannel.h"
#include "audioerror.h"

#include <QDebug>

AudioChannel::AudioChannel(int channel_id, int device_id, QObject *parent)
    : QObject{parent}
    , channel{channel_id}
    , device{device_id}
{}

AudioChannel::~AudioChannel()
{
  if (!BASS_ChannelStop(stream))
  {
    qDebug() << "Failed to stop bass Channel:" << AudioError::getErrorMessage();
  }
}

void AudioChannel::setFile(QString f_file)
{
  if (f_file.startsWith("http"))
  {
    stream = BASS_StreamCreateURL(f_file.toUtf8(), 0, 0, nullptr, nullptr);
  }
  else
  {
    stream = BASS_StreamCreateFile(false, f_file.toUtf8(), 0, 0, BASS_SAMPLE_FLOAT | BASS_SAMPLE_LOOP);
  }

  if (!stream)
  {
    qDebug() << "Failed to create stream:" << AudioError::getErrorMessage();
  }
  setChannelDevice(device);
}

void AudioChannel::setLoopPoints(quint32 start, quint32 end)
{
  loop_start = start;
  loop_end = end;
}

void AudioChannel::setChannelVolume(int volume)
{
  float f_volume = (std::clamp(volume, 0, 100) / 100.0f);
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
