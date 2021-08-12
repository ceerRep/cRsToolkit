#include "platformmanager.h"

#include "bilibiliplatform.h"
#include "huyaplatform.h"
#include "douyuplatform.h"

PlatformManager::PlatformManager()
{
    addPlatform(new BilibiliPlatform);
    addPlatform(new HuyaPlatform);
    addPlatform(new DouyuPlatform);
}

void PlatformManager::addPlatform(Platform *platform)
{
    platforms[platform->getName()] = platform;
}

Platform* PlatformManager::getPlatform(const QString& name)
{
    if (platforms.contains(name))
        return platforms[name];
    qDebug() << "Unknown platform: " << name;

    throw std::exception("Unknown platform");
}

PlatformManager& PlatformManager::getInstance()
{
    if (!manager)
        manager = new PlatformManager();
    return *manager;
}
