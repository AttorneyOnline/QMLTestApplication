#pragma once

#include <QObject>

#include "bass.h"

class AudioChannel : public QObject
{
public:
  explicit AudioChannel(int channel_id, int device_id, QObject *parent = nullptr);
  ~AudioChannel();

  void setFile(QString f_file);
  void setLoopPoints(quint32 start, quint32 end);
  void setChannelVolume(int volume);
  void setChannelDevice(DWORD device_id);
  void start();
  void stop();
  void CALLBACK endSyncProc(HSYNC handle, DWORD channel, DWORD data, void *user);

private:
  HSTREAM stream;
  quint32 loop_start;
  quint32 loop_end;
  int channel;
  int device;
};
