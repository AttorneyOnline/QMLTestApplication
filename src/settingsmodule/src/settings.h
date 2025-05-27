#pragma once

#include <QObject>
#include <QQmlEngine>

class SettingsBackend;
class Settings : public QObject
{
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

public:
  explicit Settings(QObject *parent = nullptr);

  Q_INVOKABLE void writeCustomValue(QString key, QVariant value);
  Q_INVOKABLE QVariant readCustomValue(QString key);

  void setBackend(SettingsBackend *f_backend);

private:
  QPointer<SettingsBackend> backend;
};
