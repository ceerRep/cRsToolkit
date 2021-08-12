#include "huyaplatform.h"

#include <QEventLoop>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QNetworkAccessManager>

HuyaPlatform::HuyaPlatform()
{
}

QString HuyaPlatform::getName() const
{
    return "huya";
}

QString HuyaPlatform::getLiveURL(const Broadcaster &broadcaster) const
{
    return "https://www.huya.com/" + QString(broadcaster.id);
}

int HuyaPlatform::getLiveState(const Broadcaster &broadcaster) const
{
    try {
        auto response = getURL(getLiveURL(broadcaster));

        int pos = response.indexOf("TT_ROOM_DATA");

        int pos1 = response.indexOf('{', pos);
        int pos2 = response.indexOf('}', pos);
        auto jsontext = response.mid(pos1, pos2 - pos1 + 1);

        auto document = QJsonDocument::fromJson(jsontext);

        auto state = document["state"].toString();

        qDebug() << broadcaster.platform << ' ' << broadcaster.id << ' ' << state;

        if (state == "ON")
            return Broadcaster::ON;
        else
            return Broadcaster::OFF;
    }  catch (...) {
        qDebug() << broadcaster.platform << ' ' << broadcaster.id << ' ' << "Exception occured";
        return Broadcaster::UNKNOWN;
    }

}
