
#pragma once

#include <QMap>
#include <QObject>
#include <QPointer>
#include <QStringList>

class AudioChannel;
class AudioSettings;

class AudioBackend : public QObject
{
  Q_OBJECT

public:
  explicit AudioBackend(QObject *parent = nullptr);
  ~AudioBackend();

  QStringList songs() const;
  QStringList devices() const;

  void play(const QString &song, int channel);
  void resume(int channel);
  void pause(int channel);
  void stop(int channel);
  int volume(int channel_id);
  void setVolume(int volume, int channel);
  QString device(int channel_id);
  void setDevice(const QString &device, int channel);

  void setFadeInEnabled(bool enabled);
  void setFadeOutEnabled(bool enabled);
  void setSynchronizeEnabled(bool enabled);

  bool fadeInEnabled();
  bool fadeOutEnabled();
  bool synchronizeEnabled();

Q_SIGNALS:
  void songsUpdated(const QStringList &songs);
  void devicesUpdated(const QStringList &devices);

private:
  void loadDevices();
  void loadSongs();
  AudioChannel *getChannel(int channel);

  QStringList m_songs;
  QStringList m_devices;
  QMap<int, AudioChannel *> m_channels;
  QPointer<AudioSettings> m_settings;
};
