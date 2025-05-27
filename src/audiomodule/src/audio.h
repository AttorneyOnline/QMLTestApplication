#pragma once

#include <QObject>
#include <QQmlEngine>

class AudioBackend;
class Audio : public QObject
{
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

public:
  explicit Audio(QObject *parent = nullptr);

  void setBackend(AudioBackend *f_backend);

private:
  QPointer<AudioBackend> backend;
};
