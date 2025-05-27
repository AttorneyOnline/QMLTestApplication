#pragma once

#include <QObject>

class QSettings;
class SettingsBackend : public QObject
{
  Q_OBJECT

public:
  SettingsBackend(QObject *parent = nullptr);
  ~SettingsBackend() = default;

  // audio
  QString audioDevice(int channel);
  void setAudioDevice(int channel, QString device);

  void writeCustomValue(QString key, QVariant value);
  QVariant readCustomValue(QString key);

private:
  QSettings *settings;

  void migrate();
};
