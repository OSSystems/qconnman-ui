#include "wirelesspage.h"
#include "connman.h"
#include "service.h"
#include "authdialog.h"
#include "technology.h"
#include "hiddennetworkdialog.h"

#include <connman/connmanservice.h>

#include <QDebug>

WirelessPage::WirelessPage(QWidget *parent):
    QWidget(parent)
{
    ui.setupUi(this);

    updateState();

    connect(ui.enableCheck, SIGNAL(clicked(bool)), SLOT(toggleTechnology(bool)));

    connect(ui.connectButton, SIGNAL(clicked()), SLOT(connectToNetwork()));
    connect(ui.disconnectButton, SIGNAL(clicked()), SLOT(disconnect()));
//    connect(ui.otherButton, SIGNAL(clicked()), SLOT(showAddNetworkDialog()));

    connect(ui.networkList, SIGNAL(currentRowChanged(int)), SLOT(networkListItemChanged(int)));

    connect(Connman::instance(), SIGNAL(enabledTechnologiesChanged()), SLOT(updateState()));
    connect(Connman::instance(), SIGNAL(connectedTechnologiesChanged()), SLOT(updateState()));
}

QString WirelessPage::connectedServicePath()
{
    Q_FOREACH (const QString &servicePath, Connman::instance()->services())
    {
        Service service(servicePath, this);
        if ((QStringList() << "ready" << "online").contains(service.state()) && service.type() == "wifi")
            return servicePath;
    }
    return QString();
}

void WirelessPage::updateState()
{
    bool enabled = Connman::instance()->isTechnologyEnabled("wifi");
    bool connected = Connman::instance()->connectedTechnologies().contains("wifi");

    ui.enableCheck->setChecked(enabled);
    ui.networkList->setEnabled(enabled);

    if (!enabled)
    {
        ui.status->setText("Disabled");
        ui.info->setText("");
    }

    QString state = Technology(Connman::instance()->technologyPath("wifi"), this).state();
    if (state == "connected" || connected)
    {
        ui.status->setText("Connected");

        Q_FOREACH (const QString &servicePath, Connman::instance()->services())
        {
            Service service(servicePath, this);
            if ((QStringList() << "ready" << "online").contains(service.state()) && service.type() == "wifi")
            {
                ui.info->setText(QString("Wireless device is connected to '%1' and has the IP Address %2")
                                 .arg(service.name())
                                 .arg(service.ipv4Settings()["Address"].toString()));
                break;
            }
        }
    }
    else if (enabled && (state == "offline" || !connected))
    {
        ui.status->setText("Disconnected");
        ui.info->setText("");
    }

    if (ui.networkList->currentRow() == -1)
    {
        ui.connectButton->setEnabled(false);
        ui.disconnectButton->setEnabled(false);
        return;
    }

    qDebug() << connectedServicePath();
    qDebug() << ui.networkList->selectedNetwork();

    Service service(connectedServicePath(), this);
    ui.connectButton->setEnabled(service.path() != ui.networkList->selectedNetwork());
    ui.disconnectButton->setEnabled(service.path() == ui.networkList->selectedNetwork());
}

void WirelessPage::networkListItemChanged(int row)
{
    if (connectedServicePath().isEmpty())
    {
        ui.connectButton->setEnabled(true);
        ui.disconnectButton->setEnabled(false);
        return;
    }

    Service service(connectedServicePath(), this);
    ui.connectButton->setEnabled(service.path() != ui.networkList->selectedNetwork());
    ui.disconnectButton->setEnabled(service.path() == ui.networkList->selectedNetwork());
}

void WirelessPage::toggleTechnology(bool checked)
{
    if (checked)
        Connman::instance()->enableTechnology("wifi");
    else
        Connman::instance()->disableTechnology("wifi");
}

void WirelessPage::connectToNetwork()
{
    Service service(ui.networkList->selectedNetwork(), this);
    service.setAutoConnect(true);
    service.connect();
}

void WirelessPage::disconnect()
{
    Service(ui.networkList->selectedNetwork(), this).disconnect();
}

void WirelessPage::showAddNetworkDialog()
{
//    AddNetworkDialog dialog(this);
//    if (dialog.exec() != QDialog::Accepted)
//        return;

//    Connman::instance()->connectService(dialog.toMap());
}
