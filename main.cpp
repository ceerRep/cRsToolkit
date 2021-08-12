#include <QIcon>
#include <QMenu>
#include <QLabel>
#include <QObject>
#include <QSettings>
#include <QApplication>
#include <QtConcurrent>
#include <QWidgetAction>
#include <QSystemTrayIcon>
#include <QDesktopServices>

#include <Windows.h>

#include "Broadcaster.hpp"
#include "platformmanager.h"

#include <QSslSocket>

QSystemTrayIcon *trayIcon;

void SetKeyboardRepeatAccel(BOOL status)
{
    FILTERKEYS filterkeys;
    memset(&filterkeys, 0, sizeof filterkeys);

    filterkeys.cbSize = sizeof filterkeys;
    filterkeys.dwFlags = !!status;
    filterkeys.iBounceMSec = 0;
    filterkeys.iDelayMSec = 400;
    filterkeys.iRepeatMSec = 1;
    filterkeys.iWaitMSec = 0;

    SystemParametersInfoA(
        SPI_SETFILTERKEYS,
        sizeof(FILTERKEYS),
        &filterkeys,
        SPIF_SENDCHANGE
    );
}

int getLiveStatus(const Broadcaster& bc)
{
    return PlatformManager::getInstance().getPlatform(bc.platform)->getLiveState(bc);
}

void updateMenuStatus(QVector<std::pair<QAction*, Broadcaster>> *actionlist)
{
    PlatformManager::getInstance();
    QVector<QFuture<int>> futures;

    for (auto&& [action, broadcaster]: *actionlist) {
        futures.append(QtConcurrent::run(getLiveStatus, broadcaster));
    }

    for (int i = 0; i < futures.size(); i++) {
        futures[i].waitForFinished();

        int result = futures[i].result();

        auto& [action, broadcaster] = (*actionlist)[i];

        if (result == Broadcaster::ON) {
            action->setIcon(QIcon(":/green.png"));

            if (broadcaster.state == Broadcaster::OFF) {
                trayIcon->showMessage("Live Notify", "Live " + broadcaster.name + " starts");
            }

            broadcaster.state = Broadcaster::ON;
        }
        else if (result == Broadcaster::OFF) {
            action->setIcon(QIcon(":/red.png"));

            broadcaster.state = Broadcaster::OFF;
        }
    }

    QTimer::singleShot(10000, [actionlist]()->void {
        updateMenuStatus(actionlist);
    });
}

void initializeLiveMenu(QMenu* menu)
{
    QSettings setting("config.ini", QSettings::IniFormat);
    setting.beginGroup("broadcasters");
    auto keys = setting.allKeys();
    auto broadcasternames = QVector<QString>();
    auto broadcasters = QVector<Broadcaster>();

    auto actionlist = new QVector<std::pair<QAction*, Broadcaster>>;

    for (auto &keyname: keys) {
        broadcasternames.append(setting.value(keyname).toString());
    }

    setting.endGroup();

    for (auto &broadcastername: broadcasternames) {
        setting.beginGroup(broadcastername);

        Broadcaster bc;
        bc.id = setting.value("id", "").toString();
        bc.name = setting.value("name", "").toString();
        bc.platform = setting.value("platform", "").toString();

        qDebug() << bc.id << ' ' << bc.name << ' ' << bc.platform;

        broadcasters.append(bc);

        setting.endGroup();
    }

    for (auto &broadcaster: broadcasters) {
        auto URL = PlatformManager::getInstance().getPlatform(broadcaster.platform)->getLiveURL(broadcaster);
        QAction *action = new QAction(broadcaster.name);
        action->setIcon(QIcon(":/red.png"));
        action->setIconVisibleInMenu(true);

        QObject::connect(action, &QAction::triggered,
                         [URL] (bool) -> void {
            QDesktopServices::openUrl(URL);
        });

        menu->addAction(action);

        actionlist->append({action, broadcaster});
    }

    QTimer::singleShot(1000, [actionlist]()->void {
        updateMenuStatus(actionlist);
    });
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSystemTrayIcon trayIcon;
    trayIcon.setIcon(QIcon(":/icon.png"));

    ::trayIcon = &trayIcon;

    QMenu *trayIconMenu = new QMenu;

    QAction *accelKeyboardAction = new QAction("Accel Keyboard");
    accelKeyboardAction->setCheckable(true);
    QObject::connect(accelKeyboardAction, &QAction::triggered, [](bool checked) -> void {
        SetKeyboardRepeatAccel(checked);
    });

    QAction *closeScreenAction = new QAction("Close Screen");
    QObject::connect(closeScreenAction, &QAction::triggered, [](bool checked) -> void {
        DefWindowProc(GetDesktopWindow(), WM_SYSCOMMAND, SC_MONITORPOWER, 2);
    });

    QAction *exitAction = new QAction("Exit");
    QObject::connect(exitAction, &QAction::triggered, [&a](bool checked) -> void {
        a.exit(0);
    });

    QMenu *liveSubMenu = trayIconMenu->addMenu("Live");

    initializeLiveMenu(liveSubMenu);

    trayIconMenu->addAction(accelKeyboardAction);
    trayIconMenu->addAction(closeScreenAction);
    trayIconMenu->addAction(exitAction);

    trayIcon.setContextMenu(trayIconMenu);

    trayIcon.show();

    return a.exec();
}
