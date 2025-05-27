#include "themebackend.h"
#include "themesettings.h"

#include <QQmlApplicationEngine>

ThemeBackend::ThemeBackend(QObject *parent, QQmlApplicationEngine *engine)
    : QObject{parent}
    , m_engine{engine}
    , m_settings{new ThemeSettings(this)}
{}

void ThemeBackend::clearComponentCache()
{
  qDebug() << "ThemeBackend: Cleared component cache.";
  m_engine->clearComponentCache();
  m_engine->trimComponentCache();
}
