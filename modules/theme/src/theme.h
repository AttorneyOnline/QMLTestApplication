#pragma once

#include <QObject>
#include <QPointer>
#include <QtQmlIntegration/qqmlintegration.h>

class ThemeBackend;
class Theme : public QObject
{
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

public:
  explicit Theme(QObject *parent = nullptr);

  void setBackend(ThemeBackend *backend);

  Q_INVOKABLE void clearComponentCache();

private:
  QPointer<ThemeBackend> m_backend;
};
