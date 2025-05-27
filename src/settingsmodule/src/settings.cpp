#include "settings.h"
#include "settingsbackend.h"

Settings::Settings(QObject *parent)
    : QObject{parent}
{}

void Settings::setBackend(SettingsBackend *f_backend)
{
  if (backend)
  {
    backend->deleteLater();
  }
  backend = f_backend;
}

void Settings::writeCustomValue(QString key, QVariant value)
{
  if (backend)
  {
    backend->writeCustomValue(key, value);
  }
}

QVariant Settings::readCustomValue(QString key)
{
  if (backend)
  {
    return backend->readCustomValue(key);
  }
  return QVariant{};
}
