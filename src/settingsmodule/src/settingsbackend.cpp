#include "settingsbackend.h"

#include <QDebug>
#include <QSettings>
#include <QtAssert>
#include <qassert.h>

const static int TARGET_VERSION = 1;
Q_GLOBAL_STATIC(QStringList, PROTECTED_KEYS, {"audio"})

SettingsBackend::SettingsBackend(QObject *parent)
    : QObject{parent}
    , settings(new QSettings("config.ini", QSettings::IniFormat, this))
{
  migrate();
}

QString SettingsBackend::audioDevice(int channel)
{
  return settings->value(("audio/channel_" + (QString::number(channel)), "default")).toString();
}

void SettingsBackend::setAudioDevice(int channel, QString device)
{
  settings->setValue("audio/channel_" + (QString::number(channel)), device);
}

void SettingsBackend::migrate()
{
  const int current_version = settings->value("version/major", 0).toInt();
  if (current_version == 0)
  {
    // This is a fresh settings file.
    settings->setValue("version/major", TARGET_VERSION);
  }
}

void SettingsBackend::writeCustomValue(QString key, QVariant value)
{
  const QString group_key = key.split("/").first();
  if (PROTECTED_KEYS->contains(group_key, Qt::CaseInsensitive))
  {
    qDebug() << "Unable to write to protected groups. Please use the appropriate getter/setter";
    return;
  }
  settings->setValue(key, value);
}

QVariant SettingsBackend::readCustomValue(QString key)
{
  const QString group_key = key.split("/").first();
  if (PROTECTED_KEYS->contains(group_key, Qt::CaseInsensitive))
  {
    qDebug() << "Unable to read protected groups. Please use the appropriate getter/setter";
    return QVariant{};
  }

  return settings->value(key, QVariant{});
}
