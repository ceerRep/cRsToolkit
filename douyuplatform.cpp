#include "douyuplatform.h"

#include <QJsonObject>
#include <QJsonDocument>

DouyuPlatform::DouyuPlatform()
{
}

QString DouyuPlatform::getName() const
{
    return "douyu";
}

QString DouyuPlatform::getLiveURL(const Broadcaster &broadcaster) const
{
    return "https://www.douyu.com/" + QString(broadcaster.id);
}

int DouyuPlatform::getLiveState(const Broadcaster &broadcaster) const
{
    try {
        auto response = getURL("https://www.douyu.com/betard/" + broadcaster.id);
        auto document = QJsonDocument::fromJson(response);

        auto roomData = document["room"].toObject();
        int showStatus = roomData["show_status"].toInt();
        int videoLoop = roomData["videoLoop"].toInt();

        if (showStatus == 1 && !videoLoop)
            return Broadcaster::ON;
        else
            return Broadcaster::OFF;
    }  catch (...) {
        qDebug() << broadcaster.platform << ' ' << broadcaster.id << ' ' << "Exception occured";
        return Broadcaster::UNKNOWN;
    }

}
