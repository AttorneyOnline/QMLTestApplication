#pragma once

#include <QString>
#include <QStringLiteral>

#include "bass.h"

namespace AudioError
{
  static int getBASSError()
  {
    return BASS_ErrorGetCode();
  }

  static QString getErrorMessage()
  {
    switch (getBASSError())
    {
    case BASS_OK:
      return QStringLiteral("all is OK");
    case BASS_ERROR_MEM:
      return QStringLiteral("memory error");
    case BASS_ERROR_FILEOPEN:
      return QStringLiteral("can't open the file");
    case BASS_ERROR_DRIVER:
      return QStringLiteral("can't find a free/valid driver");
    case BASS_ERROR_BUFLOST:
      return QStringLiteral("the sample buffer was lost");
    case BASS_ERROR_HANDLE:
      return QStringLiteral("invalid handle");
    case BASS_ERROR_FORMAT:
      return QStringLiteral("unsupported sample format");
    case BASS_ERROR_POSITION:
      return QStringLiteral("invalid position");
    case BASS_ERROR_INIT:
      return QStringLiteral("BASS_Init has not been successfully called");
    case BASS_ERROR_START:
      return QStringLiteral("BASS_Start has not been successfully called");
    case BASS_ERROR_SSL:
      return QStringLiteral("SSL/HTTPS support isn't available");
    case BASS_ERROR_REINIT:
      return QStringLiteral("device needs to be reinitialized");
    case BASS_ERROR_ALREADY:
      return QStringLiteral("already initialized/paused/whatever");
    case BASS_ERROR_NOTAUDIO:
      return QStringLiteral("file does not contain audio");
    case BASS_ERROR_NOCHAN:
      return QStringLiteral("can't get a free channel");
    case BASS_ERROR_ILLTYPE:
      return QStringLiteral("an illegal type was specified");
    case BASS_ERROR_ILLPARAM:
      return QStringLiteral("an illegal parameter was specified");
    case BASS_ERROR_NO3D:
      return QStringLiteral("no 3D support");
    case BASS_ERROR_NOEAX:
      return QStringLiteral("no EAX support");
    case BASS_ERROR_DEVICE:
      return QStringLiteral("illegal device number");
    case BASS_ERROR_NOPLAY:
      return QStringLiteral("not playing");
    case BASS_ERROR_FREQ:
      return QStringLiteral("illegal sample rate");
    case BASS_ERROR_NOTFILE:
      return QStringLiteral("the stream is not a file stream");
    case BASS_ERROR_NOHW:
      return QStringLiteral("no hardware voices available");
    case BASS_ERROR_EMPTY:
      return QStringLiteral("the file has no sample data");
    case BASS_ERROR_NONET:
      return QStringLiteral("no internet connection could be opened");
    case BASS_ERROR_CREATE:
      return QStringLiteral("couldn't create the file");
    case BASS_ERROR_NOFX:
      return QStringLiteral("effects are not available");
    case BASS_ERROR_NOTAVAIL:
      return QStringLiteral("requested data/action is not available");
    case BASS_ERROR_DECODE:
      return QStringLiteral("the channel is/isn't a \"decoding channel\"");
    case BASS_ERROR_DX:
      return QStringLiteral("a sufficient DirectX version is not installed");
    case BASS_ERROR_TIMEOUT:
      return QStringLiteral("connection timedout");
    case BASS_ERROR_FILEFORM:
      return QStringLiteral("unsupported file format");
    case BASS_ERROR_SPEAKER:
      return QStringLiteral("unavailable speaker");
    case BASS_ERROR_VERSION:
      return QStringLiteral("invalid BASS version (used by add-ons)");
    case BASS_ERROR_CODEC:
      return QStringLiteral("codec is not available/supported");
    case BASS_ERROR_ENDED:
      return QStringLiteral("the channel/file has ended");
    case BASS_ERROR_BUSY:
      return QStringLiteral("the device is busy");
    case BASS_ERROR_UNSTREAMABLE:
      return QStringLiteral("unstreamable file");
    case BASS_ERROR_PROTOCOL:
      return QStringLiteral("unsupported protocol");
    case BASS_ERROR_DENIED:
      return QStringLiteral("access denied");
    case BASS_ERROR_UNKNOWN:
      return QStringLiteral("some other mystery problem");
    default:
      return QStringLiteral("unknown error code");
    }
  }
};
