#pragma once

#include <QObject>
#include <QPointer>

class QSettings;

class ThemeSettings : public QObject
{
  Q_OBJECT

public:
  explicit ThemeSettings(QObject *parent = nullptr);

private:
  QPointer<QSettings> settings;
};
