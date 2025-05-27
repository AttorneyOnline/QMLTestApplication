#include "themesettings.h"

#include <QDebug>
#include <QSettings>

ThemeSettings::ThemeSettings(QObject *parent)
    : QObject{parent}
    , settings{new QSettings("theme.ini", QSettings::IniFormat, this)}
{}
