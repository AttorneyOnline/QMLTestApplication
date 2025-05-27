#include "theme.h"
#include "themebackend.h"

#include <QDebug>

Theme::Theme(QObject *parent)
    : QObject{parent}
{}

void Theme::setBackend(ThemeBackend *backend)
{
  if (m_backend)
  {
    qDebug() << "Theme: Replacing existing backend";
    m_backend.get()->deleteLater();
    m_backend.clear();
  }

  m_backend = backend;
  qDebug() << "Theme: Backend set successfully";
}

void Theme::clearComponentCache()
{
  if (!m_backend)
  {
    qDebug() << "Theme: Failed to clear component cache - Backend not installed";
    return;
  }

  m_backend->clearComponentCache();
}
