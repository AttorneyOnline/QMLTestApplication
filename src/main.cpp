#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "audiomodule/src/audio.h"
#include "audiomodule/src/audiobackend.h"
#include "settingsmodule/src/settings.h"
#include "settingsmodule/src/settingsbackend.h"

int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;

  const QUrl url = QUrl::fromLocalFile(QStringLiteral("../ressources/main.qml"));
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl) {
    if (!obj && url == objUrl)
      QCoreApplication::exit(-1);
  }, Qt::QueuedConnection);

  auto control = engine.singletonInstance<Audio *>("Attorney.Audio", "Audio");
  control->setBackend(new AudioBackend(&engine));

  auto settings = engine.singletonInstance<Settings *>("Attorney.Settings", "Settings");
  settings->setBackend(new SettingsBackend(&engine));

  engine.load(url);
  return app.exec();
}
