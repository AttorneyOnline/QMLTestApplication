#pragma once

#include <QObject>

class QQmlApplicationEngine;
class ThemeSettings;

class ThemeBackend : public QObject
{
  Q_OBJECT
public:
  explicit ThemeBackend(QObject *parent = nullptr, QQmlApplicationEngine *engine = nullptr);

  void clearComponentCache();

private:
  QQmlApplicationEngine *m_engine;
  ThemeSettings *m_settings;
};
