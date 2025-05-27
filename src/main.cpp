#include "../modules/audio/src/audio.h"
#include "../modules/audio/src/audiobackend.h"
#include "../modules/theme/src/theme.h"
#include "../modules/theme/src/themebackend.h"
#include <QApplication>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <kddockwidgets/Config.h>
#include <kddockwidgets/qtquick/Platform.h>

#include <QDebug>
#include <QTimer>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  QQmlApplicationEngine engine;
  KDDockWidgets::initFrontend(KDDockWidgets::FrontendType::QtQuick);

  KDDockWidgets::Config::Flags bar;
  KDDockWidgets::Config::self().setFlags(bar);
  KDDockWidgets::Config::self().setSeparatorThickness(5);

  qDebug() << bar;

  KDDockWidgets::QtQuick::Platform::instance()->setQmlEngine(&engine);

  const QUrl url = QUrl::fromLocalFile(QStringLiteral("./files/system/main.qml"));
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl) {
    if (!obj && url == objUrl)
      QCoreApplication::exit(-1);
  }, Qt::QueuedConnection);

  auto audio_singleton = engine.singletonInstance<Audio *>("Attorney.Audio", "Audio");
  audio_singleton->setBackend(new AudioBackend(&engine));

  auto theme_singleton = engine.singletonInstance<Theme *>("Attorney.Theme", "Theme");
  theme_singleton->setBackend(new ThemeBackend(&engine, &engine));

  engine.load(url);
  return app.exec();
}
