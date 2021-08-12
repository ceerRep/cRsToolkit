#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include <exception>

#include <QUrl>
#include <QEventLoop>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include <QMessageBox>

#include <Broadcaster.hpp>

class Platform
{
public:
    virtual QString getName() const = 0;
    virtual QString getLiveURL(const Broadcaster& broadcaster) const = 0;
    virtual int getLiveState(const Broadcaster& broadcaster) const = 0;

    static inline QByteArray getURL(QUrl url) {
        QEventLoop loop;

        QNetworkAccessManager manager;
        QObject::connect(&manager, &QNetworkAccessManager::finished,
                         [&loop](QNetworkReply *p) -> void { loop.quit(); });

        auto request = QNetworkRequest(url);
        request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:59.0) Gecko/20100101 Firefox/59.0");
        request.setRawHeader("accept", "*/*");

        auto *reply = manager.get(request);
        QObject::connect(reply, &QNetworkReply::redirected,
                         [reply](auto url) -> void {
            qDebug() << "Redirect: " << url;
            emit reply->redirectAllowed();
        });

        loop.exec();

        if (reply->error() != reply->NoError)
        {
            auto errorString = QString("Error: ") + reply->errorString();
            auto utf8 = errorString.toStdString();
            qDebug() << "Error: " << reply->url() << ' ' << reply->error() << ' ' << reply->errorString();

            reply->deleteLater();

            throw std::exception(utf8.c_str());
        }

        auto ret = reply->readAll();
        reply->deleteLater();
        return ret;
    }
};

#endif // PLATFORM_HPP
