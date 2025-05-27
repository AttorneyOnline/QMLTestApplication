#include "audiosettings.h"

#include <QSettings>

namespace
{
  constexpr int DEFAULT_VOLUME = 100;
  constexpr int DEFAULT_FADE_DURATION = 4000;
} // namespace

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
  settings->sync();
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
  settings->sync();
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
  settings->sync();
}

int AudioSettings::fadeInDuration(const QString &song)
{
  const QString filename = song + ".txt";
  QSettings config(filename, QSettings::IniFormat, nullptr);

  if (config.status() != QSettings::NoError)
  {
    return DEFAULT_FADE_DURATION;
  }

  return settings->value("fadein_duration", DEFAULT_FADE_DURATION).toInt();
}

int AudioSettings::fadeOutDuration(const QString &song)
{
  const QString filename = song + ".txt";
  QSettings config(filename, QSettings::IniFormat, nullptr);

  if (config.status() != QSettings::NoError)
  {
    return DEFAULT_FADE_DURATION;
  }

  return settings->value("fadeout_duration", DEFAULT_FADE_DURATION).toInt();
}

QPair<double, double> AudioSettings::loopPoints(const QString &song) const
{
  const QString filename = song + ".txt";
  QSettings config(filename, QSettings::IniFormat, nullptr);

  if (config.status() != QSettings::NoError)
  {
    return {0, 0};
  }

  QPair<double, double> points;
  points.first = config.value("loop_start", 0).toDouble();
  points.second = config.value("loop_end", 0).toDouble();

  return points;
}

bool AudioSettings::fadeOutEnabled() const
{
  return settings->value("effects/fadeout", true).toBool();
}

void AudioSettings::setFadeOutEnabled(bool state)
{
  settings->setValue("effects/fadeout", state);
}

bool AudioSettings::fadeInEnabled() const
{
  return settings->value("effects/fadein", false).toBool();
}

void AudioSettings::setFadeInEnabled(bool state)
{
  settings->setValue("effects/fadein", state);
}

bool AudioSettings::synchronizeEnabled() const
{
  return settings->value("effects/synchronize", false).toBool();
}

void AudioSettings::setSynchronizedEnabled(bool state)
{
  settings->setValue("effects/synchronize", state);
}
