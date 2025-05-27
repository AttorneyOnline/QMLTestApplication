#pragma once

#include "bass.h"
#include <QObject>

class AudioChannel : public QObject
{
  Q_OBJECT

public:
  explicit AudioChannel(QObject *parent = nullptr);
  ~AudioChannel();

  void setSong(const QString &file_path);
  void setDevice(DWORD device_id);
  void setVolume(int target_volume);
  void setLoopPoints(double start, double end);
  void setEnabled(bool state);

  void start();
  void pause();
  void stop();

  void fadeOut(int duration);
  void fadeIn(int duration);
  const QString song();
  QWORD position();
  void setPosition(QWORD byte_position);

private:
  QString m_song;
  HSTREAM stream;
  int audio_volume;
  bool audio_enabled;
  const float calculateVolume();
};
