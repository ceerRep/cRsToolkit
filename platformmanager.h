#ifndef PLATFORMMANAGER_H
#define PLATFORMMANAGER_H

#include "Platform.hpp"

#include <QMap>

class PlatformManager
{
    static inline PlatformManager* manager = nullptr;

    QMap<QString, Platform*> platforms;

    PlatformManager();
public:

    void addPlatform(Platform*);
    Platform* getPlatform(const QString&);

    static PlatformManager& getInstance();
};

#endif // PLATFORMMANAGER_H
