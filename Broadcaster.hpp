#ifndef BROADCASTER_HPP
#define BROADCASTER_HPP

#include <QString>

struct Broadcaster {
    enum {
        UNKNOWN,
        OFF,
        ON
    };

    int state;
    QString id;
    QString name;
    QString platform;

    Broadcaster() : Broadcaster("", "", "") {}

    Broadcaster(QString id, QString name, QString platform)
        : state(UNKNOWN), id(id), name(name), platform(platform)
    {}
};

#endif // BROADCASTER_HPP
