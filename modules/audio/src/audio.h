#pragma once

#include <QObject>
#include <QProperty>
#include <QQmlEngine>

class AudioBackend;
class Audio : public QObject
{
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

  Q_PROPERTY(bool fadeOut READ fadeOut WRITE setFadeOut NOTIFY fadeOutChanged BINDABLE bindableFadeOut)
  Q_PROPERTY(bool fadeIn READ fadeIn WRITE setFadeIn NOTIFY fadeInChanged BINDABLE bindableFadeIn)
  Q_PROPERTY(bool synchronize READ synchronize WRITE setSynchronize NOTIFY synchronizeChanged BINDABLE bindableSynchronize)
  Q_PROPERTY(QStringList musiclist READ musiclist WRITE setMusiclist NOTIFY musiclistChanged BINDABLE bindableMusiclist)
  Q_PROPERTY(QStringList audioDevices READ audioDevices WRITE setAudioDevices NOTIFY audioDevicesChanged BINDABLE bindableAudioDevices)

  Q_PROPERTY(int channel0Volume READ channel0Volume WRITE setChannel0Volume NOTIFY channel0VolumeChanged BINDABLE bindableChannel0Volume)
  Q_PROPERTY(int channel1Volume READ channel1Volume WRITE setChannel1Volume NOTIFY channel1VolumeChanged BINDABLE bindableChannel1Volume)
  Q_PROPERTY(int channel2Volume READ channel2Volume WRITE setChannel2Volume NOTIFY channel2VolumeChanged BINDABLE bindableChannel2Volume)
  Q_PROPERTY(int channel3Volume READ channel3Volume WRITE setChannel3Volume NOTIFY channel3VolumeChanged BINDABLE bindableChannel3Volume)

  Q_PROPERTY(QString channel0Device READ channel0Device WRITE setChannel0Device NOTIFY channel0DeviceChanged BINDABLE bindableChannel0Device)
  Q_PROPERTY(QString channel1Device READ channel1Device WRITE setChannel1Device NOTIFY channel1DeviceChanged BINDABLE bindableChannel1Device)
  Q_PROPERTY(QString channel2Device READ channel2Device WRITE setChannel2Device NOTIFY channel2DeviceChanged BINDABLE bindableChannel2Device)
  Q_PROPERTY(QString channel3Device READ channel3Device WRITE setChannel3Device NOTIFY channel3DeviceChanged BINDABLE bindableChannel3Device)

public:
  explicit Audio(QObject *parent = nullptr);
  void setBackend(AudioBackend *f_backend);

  // Welcome to the boilerplate section to update properties.

  bool fadeOut() const;
  void setFadeOut(bool enabled);
  QBindable<bool> bindableFadeOut() const;

  bool fadeIn() const;
  void setFadeIn(bool enabled);
  QBindable<bool> bindableFadeIn() const;

  bool synchronize() const;
  void setSynchronize(bool enabled);
  QBindable<bool> bindableSynchronize() const;

  QStringList musiclist() const;
  void setMusiclist(const QStringList &musiclist);
  QBindable<QStringList> bindableMusiclist() const;

  QStringList audioDevices() const;
  void setAudioDevices(const QStringList &audioDevices);
  QBindable<QStringList> bindableAudioDevices() const;

  // ---------- VOLUME BINDINGS ----------

  int channel0Volume() const;
  void setChannel0Volume(int volume);
  QBindable<int> bindableChannel0Volume() const;

  int channel1Volume() const;
  void setChannel1Volume(int volume);
  QBindable<int> bindableChannel1Volume() const;

  int channel2Volume() const;
  void setChannel2Volume(int volume);
  QBindable<int> bindableChannel2Volume() const;

  int channel3Volume() const;
  void setChannel3Volume(int volume);
  QBindable<int> bindableChannel3Volume() const;

  // ---------- DEVICE Bindings ----------

  QString channel0Device() const;
  void setChannel0Device(const QString &device);
  QBindable<QString> bindableChannel0Device() const;

  QString channel1Device() const;
  void setChannel1Device(const QString &device);
  QBindable<QString> bindableChannel1Device() const;

  QString channel2Device() const;
  void setChannel2Device(const QString &device);
  QBindable<QString> bindableChannel2Device() const;

  QString channel3Device() const;
  void setChannel3Device(const QString &device);
  QBindable<QString> bindableChannel3Device() const;

  // Thank you for visiting the boilerplate section to update properties.

  Q_INVOKABLE void playSong(const QString &song, int channel_id);
  Q_INVOKABLE void resumeSong(int channel_id);
  Q_INVOKABLE void pauseSong(int channel_id);
  Q_INVOKABLE void stopSong(int channel_id);
  Q_INVOKABLE void setDevice(const QString &device, int channel_id);
  Q_INVOKABLE void setVolume(int volume, int channel_id);

Q_SIGNALS:
  void fadeOutChanged();
  void fadeInChanged();
  void synchronizeChanged();
  void musiclistChanged();
  void audioDevicesChanged();

  void channel0VolumeChanged();
  void channel1VolumeChanged();
  void channel2VolumeChanged();
  void channel3VolumeChanged();

  void channel0DeviceChanged();
  void channel1DeviceChanged();
  void channel2DeviceChanged();
  void channel3DeviceChanged();

private Q_SLOTS:
  void musicListUpdated(const QStringList &musiclist);
  void audioDevicesUpdated(const QStringList &audioDevices);

private:
  void updateAllOptions();
  void connectSignals();

  Q_OBJECT_BINDABLE_PROPERTY(Audio, bool, fadeOutProp, &Audio::fadeOutChanged)
  Q_OBJECT_BINDABLE_PROPERTY(Audio, bool, fadeInProp, &Audio::fadeInChanged)
  Q_OBJECT_BINDABLE_PROPERTY(Audio, bool, synchronizeProp, &Audio::synchronizeChanged)
  Q_OBJECT_BINDABLE_PROPERTY(Audio, QStringList, musiclistProp, &Audio::musiclistChanged)
  Q_OBJECT_BINDABLE_PROPERTY(Audio, QStringList, audioDevicesProp, &Audio::audioDevicesChanged)

  Q_OBJECT_BINDABLE_PROPERTY(Audio, int, channel0VolumeProp, &Audio::channel0VolumeChanged)
  Q_OBJECT_BINDABLE_PROPERTY(Audio, int, channel1VolumeProp, &Audio::channel1VolumeChanged)
  Q_OBJECT_BINDABLE_PROPERTY(Audio, int, channel2VolumeProp, &Audio::channel2VolumeChanged)
  Q_OBJECT_BINDABLE_PROPERTY(Audio, int, channel3VolumeProp, &Audio::channel3VolumeChanged)

  Q_OBJECT_BINDABLE_PROPERTY(Audio, QString, channel0DeviceProp, &Audio::channel0DeviceChanged)
  Q_OBJECT_BINDABLE_PROPERTY(Audio, QString, channel1DeviceProp, &Audio::channel1DeviceChanged)
  Q_OBJECT_BINDABLE_PROPERTY(Audio, QString, channel2DeviceProp, &Audio::channel2DeviceChanged)
  Q_OBJECT_BINDABLE_PROPERTY(Audio, QString, channel3DeviceProp, &Audio::channel3DeviceChanged)

  QPointer<AudioBackend> m_backend;
};
