#pragma once

#include <QObject>
#include <QPointer>

#include "../libs/bass.h"

class AudioChannel;

class AudioBackend : public QObject
{
public:
  explicit AudioBackend(QObject *parent);
  ~AudioBackend();

  void setChannelVolume(int channel, int volume);

private:
  HSTREAM handle;
  QVector<AudioChannel *> channels;
};
