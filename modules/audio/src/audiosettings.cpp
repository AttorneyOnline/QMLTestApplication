#include "audiosettings.h"

#include <QSettings>

#define DEFAULT_VOLUME 100

AudioSettings::AudioSettings(QObject *parent)
    : QObject{parent}
    , settings{new QSettings("audio.ini", QSettings::IniFormat, this)}
{
  qDebug() << "Status of AudioSettings" << settings->status();
}

QString AudioSettings::defaultAudioDevice()
{
  return settings->value("devices/default", "Default").toString();
}

void AudioSettings::setDefaultAudioDevice(const QString &device_name)
{
  settings->setValue("devices/default", device_name);
}

QString AudioSettings::channelAudioDevice(int id)
{
  const QString key = "devices/" + QString::number(id);
  return settings->value(key, "Default").toString();
}

void AudioSettings::setChannelAudioDevice(int id, const QString &device_name)
{
  const QString key = "devices/" + QString::number(id);
  settings->setValue(key, device_name);
}

int AudioSettings::channelVolume(int id)
{
  const QString key = "volume/" + QString::number(id);
  return settings->value(key, DEFAULT_VOLUME).toInt();
}

void AudioSettings::setChannelVolume(int id, int volume)
{
  const QString key = "volume/" + QString::number(id);
  settings->setValue(key, volume);
}
