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

  // TODO : Turn this into proper bindings down the line. Good enough for testing.
  Q_INVOKABLE QStringList availableSongs() const;
  Q_INVOKABLE QStringList availableDevices() const;

  Q_INVOKABLE int volume(int channel_id);
  Q_INVOKABLE QString device(int channel_id);
  Q_INVOKABLE bool fadeOut();
  Q_INVOKABLE bool fadeIn();

  Q_INVOKABLE void stop(int channel_id);
  Q_INVOKABLE void pause(int channel_id);
  Q_INVOKABLE void resume(int channel_id);
  Q_INVOKABLE void setDevice(int channel_id, const QString &device);
  Q_INVOKABLE void setSong(int channel_id, const QString &song);
  Q_INVOKABLE void setVolume(int channel_id, int volume);
  Q_INVOKABLE void setFadeOut(bool state);
  Q_INVOKABLE void setFadeIn(bool state);

private:
  QPointer<AudioBackend> m_backend;
};
