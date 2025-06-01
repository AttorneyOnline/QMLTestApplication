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

  QStringList availableSongs() const;
  QStringList availableDevices() const;
  QString device(int channel_id) const;
  int volume(int channel_id) const;
  bool fadeOut() const;
  bool fadeIn() const;

  void setChannelSong(int channel_id, const QString &song_path);
  void setChannelDevice(int channel_id, const QString &device_name);
  void setChannelVolume(int channel_id, int volume);
  void pauseChannel(int channel_id);
  void resumeChannel(int channel_id);
  void stopChannel(int channel_id);
  void setFadeOut(bool state);
  void setFadeIn(bool state);

private:
  void initializeAudioDevices();

  // Channel management
  AudioChannel *getChannel(int channel_id) const;
  void replaceChannel(int channel_id, AudioChannel *new_channel);

  QMap<QString, int> m_audio_devices;
  QMap<int, AudioChannel *> m_channels;
  QPointer<AudioSettings> m_settings;
};
