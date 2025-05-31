#ifndef AUDIOSETTINGS_H
#define AUDIOSETTINGS_H

#include <QObject>

class QSettings;

class AudioSettings : public QObject
{
  Q_OBJECT
public:
  explicit AudioSettings(QObject *parent = nullptr);

  QString defaultAudioDevice();
  void setDefaultAudioDevice(const QString &device_name);

  QString channelAudioDevice(int id);
  void setChannelAudioDevice(int id, const QString &device_name);

  int channelVolume(int id);
  void setChannelVolume(int id, int volume);

private:
  QSettings *settings;
};

#endif // AUDIOSETTINGS_H
