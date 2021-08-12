#include "bilibiliplatform.h"

#include <QEventLoop>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QNetworkAccessManager>

BilibiliPlatform::BilibiliPlatform()
{
}

QString BilibiliPlatform::getName() const
{
    return "bilibili";
}

QString BilibiliPlatform::getLiveURL(const Broadcaster &broadcaster) const
{
    return "https://live.bilibili.com/" + QString(broadcaster.id);
}

int BilibiliPlatform::getLiveState(const Broadcaster &broadcaster) const
{
    try {
        auto response = getURL(getLiveURL(broadcaster));

        int pos = response.indexOf("<script>window.__NEPTUNE_IS_MY_WAIFU__=");

        int pos1 = response.indexOf("live_status", pos);

        auto state = response[pos1 + 13];

        qDebug() << broadcaster.platform << ' ' << broadcaster.id << ' ' << state;

        if (state == '1')
            return Broadcaster::ON;
        else
            return Broadcaster::OFF;
    }  catch (...) {
        qDebug() << broadcaster.platform << ' ' << broadcaster.id << ' ' << "Exception occured";
        return Broadcaster::UNKNOWN;
    }

}
