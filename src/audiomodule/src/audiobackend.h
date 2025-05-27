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

  enum Channel
  {
    MUSIC,
    AMBIENCE,
    AUXILIARY,
    AUXILIARY2
  };

private:
  HSTREAM handle;
  QVector<AudioChannel *> channels;
};
