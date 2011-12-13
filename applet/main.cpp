/*
 * qconnman - Connman Applet
 * Copyright (C) 2011 O.S. Systems
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QLibraryInfo>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QWidgetAction>
#include <QTimer>
#include <QMessageBox>
#include <QProcess>
#include <QDebug>

#include "connman.h"
#include "agent.h"
#include "service.h"
#include "wirelessnetworkwidget.h"

class TrayIcon: public QObject
{
    Q_OBJECT

public:
    TrayIcon(): QObject()
    {
        m_trayIcon = new QSystemTrayIcon;
        m_trayIcon->setIcon(QIcon(QPixmap(":/images/network-wireless.png")));
        m_trayIcon->show();

        connect(Connman::instance(), SIGNAL(technologiesChanged()), SLOT(updateMenu()));
        connect(Connman::instance(), SIGNAL(servicesChanged()), SLOT(updateMenu()));

        QTimer *timer = new QTimer(this);
        timer->start(30000);
        connect(timer, SIGNAL(timeout()), SLOT(scan()));
        scan();

        new Agent(this);
        Connman::instance()->registerAgent("/");
     }

private slots:
    void scan()
    {
        Connman::instance()->requestScan("wifi");
    }

    void updateMenu()
    {
        if (m_trayIcon->contextMenu())
            m_trayIcon->contextMenu()->deleteLater();

        QMenu *menu = new QMenu();
        connect(menu, SIGNAL(triggered(QAction *)), SLOT(execAction(QAction *)));

        QStringList services = Connman::instance()->services();
        foreach (const QString &service/*, QStringList() << "ae")*/, services)
        {
            if (Connman::instance()->serviceType(service) != "wifi")
                continue;

            QWidgetAction *networkAction = new QWidgetAction(menu);
            networkAction->setData(service);
            networkAction->setDefaultWidget(new WirelessNetworkWidget(service, menu));
            
            menu->addAction(networkAction);
        }

        menu->addSeparator();

        QAction *managerAction = menu->addAction(QIcon(QPixmap(":/images/preferences-system-network.png")), tr("Manage Network Configuration"));
        managerAction->setIconVisibleInMenu(true);
        managerAction->setData("manager-network");

        m_trayIcon->setContextMenu(menu);
    }

    void execAction(QAction *action)
    {
        QString data = action->data().toString();
        if (data == "manager-network")
        {
            QProcess::startDetached("qconnman");
            return;
        }

        QStringList services = Connman::instance()->services();
        foreach (const QString &servicePath, services)
        {
            if (Connman::instance()->serviceType(servicePath) != "wifi")
                continue;

            Service service(servicePath, this);
            if (!(QStringList() << "association" << "configuration").contains(service.state()))
                continue;
            
            if (servicePath != action->data().toString())
                QMessageBox::warning(NULL, tr("Unable to connect"), tr("Can't connect to this network while another connection is being established."));

            return;
        }

        Service service(action->data().toString(), this);
        service.setAutoConnect(true);
        service.connect();
    }

private:
    QSystemTrayIcon *m_trayIcon;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    QTranslator *appTranslator = new QTranslator;
    appTranslator->load(QString("%1/../share/qconnman/i18n/%2")
                        .arg(QCoreApplication::applicationDirPath())
                        .arg(QLocale::system().name()));
    app.installTranslator(appTranslator);

    Connman::instance()->init();

    new TrayIcon();

    return app.exec();
}

#include "main.moc"
