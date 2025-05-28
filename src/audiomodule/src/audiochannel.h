#pragma once

#include <QObject>

#include "bass.h"

class AudioChannel : public QObject
{
public:
  explicit AudioChannel(int channel_id, int device_id, QObject *parent = nullptr);
  ~AudioChannel();

  void setFile(QString f_file);
  void setLoopPoints(double start, double end);
  void setChannelVolume(int volume);
  void setChannelDevice(DWORD device_id);
  void start();
  void stop();
  void setAudioEnabled(bool enabled);

private:
  HSTREAM stream;
  quint32 loop_start;
  quint32 loop_end;
  int volume;
  int channel;
  int device;
  bool audio_enabled;
};
