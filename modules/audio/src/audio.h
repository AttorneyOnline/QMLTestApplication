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

  Q_INVOKABLE QList<QString> devices();
  Q_INVOKABLE QList<QString> songs();
  Q_INVOKABLE void setDevicePerChannel(int channel, QString device);
  Q_INVOKABLE void pauseChannel(int channel);
  Q_INVOKABLE void resumeChannel(int channel);
  Q_INVOKABLE void setChannelSong(int channel, QString song);
  Q_INVOKABLE void setChannelVolume(int channel, int volume);

private:
  QPointer<AudioBackend> backend;
};
