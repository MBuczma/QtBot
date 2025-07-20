#pragma once

#include <QHash>
#include <QString>
#include <Windows.h>

class KeyMap
{
public:
    static const QHash<QString, WPARAM> &getMap();
    static QString getKeyText(WPARAM vkCode);
};
