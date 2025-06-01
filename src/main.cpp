#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "../modules/audio/src/audio.h"
#include "../modules/audio/src/audiobackend.h"

int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;

  const QUrl url = QUrl::fromLocalFile(QStringLiteral("./main.qml"));
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl) {
    if (!obj && url == objUrl)
      QCoreApplication::exit(-1);
  }, Qt::QueuedConnection);

  auto control = engine.singletonInstance<Audio *>("Attorney.Audio", "Audio");
  control->setBackend(new AudioBackend(&engine));

  engine.load(url);
  return app.exec();
}
