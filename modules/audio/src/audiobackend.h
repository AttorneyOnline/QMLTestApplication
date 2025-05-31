#pragma once

#include <QObject>
#include <QPointer>

#include "../libs/bass.h" // WHY?????? This makes me unreasonably unhappy.

class AudioChannel;

class AudioBackend : public QObject
{
public:
  explicit AudioBackend(QObject *parent);
  ~AudioBackend();

  void setChannelSong(int id, QString song);
  void setChannelVolume(int id, int volume);
  void pauseChannel(int id);
  void resumeChannel(int id);

private:
  HSTREAM handle;
  QVector<AudioChannel *> channels;
};
