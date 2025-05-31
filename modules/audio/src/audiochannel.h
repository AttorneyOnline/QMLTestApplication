#pragma once

#include "bass.h"
#include <QObject>

class AudioChannel : public QObject
{
  Q_OBJECT

public:
  explicit AudioChannel(int channel_id, int device_id, QObject *parent = nullptr);
  ~AudioChannel();

  // File and playback control
  void setFile(const QString &file_path);
  void start();
  void pause();
  void stop();
  void fadeIn(int duration);
  void fadeOut(int duration);

  // Audio configuration
  void setLoopPoints(double start, double end);
  void setVolume(int volume);
  void setDevice(DWORD device_id);
  void setEnabled(bool enabled);

private:
  float calculateTargetVolume() const;

  HSTREAM stream;
  quint32 loop_start;
  quint32 loop_end;

  // Properties
  int volume;
  int channel;
  int device;
  bool audio_enabled;
};
