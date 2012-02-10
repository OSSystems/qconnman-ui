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

#include "wirelesspage.h"
#include "connman.h"
#include "service.h"
#include "authdialog.h"
#include "technology.h"
#include "hiddennetworkdialog.h"

#include <dbus/connmanservice.h>

#include <QMessageBox>
#include <QDebug>

WirelessPage::WirelessPage(const QString &technology, QWidget *parent):
    QWidget(parent),
    m_technology(technology)
{
    ui.setupUi(this);

    updateState();

    connect(ui.enableCheck, SIGNAL(clicked(bool)), SLOT(toggleTechnology(bool)));

    connect(ui.connectButton, SIGNAL(clicked()), SLOT(connectToNetwork()));
    connect(ui.disconnectButton, SIGNAL(clicked()), SLOT(disconnect()));

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
        ui.status->setText(tr("Disabled"));
        ui.info->setText("");
    }

    QString state = Technology(Connman::instance()->technologyPath("wifi"), this).state();
    if (state == "connected" || connected)
    {
        ui.status->setText(tr("Connected"));

        Q_FOREACH (const QString &servicePath, Connman::instance()->services())
        {
            Service service(servicePath, this);
            if ((QStringList() << "ready" << "online").contains(service.state()) && service.type() == "wifi")
            {
                ui.info->setText(tr("Wireless device is connected to '%1' and has the IP Address %2")
                                 .arg(service.name())
                                 .arg(service.ipv4Settings()["Address"].toString()));
                break;
            }
        }
    }
    else if (enabled && (state == "offline" || !connected))
    {
        ui.status->setText(tr("Disconnected"));
        ui.info->setText("");
    }

    if (ui.networkList->currentRow() == -1)
    {
        ui.connectButton->setEnabled(false);
        ui.disconnectButton->setEnabled(false);
        return;
    }

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
    QStringList services = Connman::instance()->services();
    foreach (const QString &servicePath, services)
    {
        if (Connman::instance()->serviceType(servicePath) != "wifi")
            continue;

        Service service(servicePath, this);
        if (!(QStringList() << "association" << "configuration").contains(service.state()))
            continue;

        if (servicePath != ui.networkList->selectedNetwork())
            QMessageBox::warning(this, tr("Unable to connect"), tr("Can't connect to this network while another connection is being established."));

        return;
    }

    Service service(ui.networkList->selectedNetwork(), this);
    service.setAutoConnect(true);
    service.connect();
}

void WirelessPage::disconnect()
{
    Service(ui.networkList->selectedNetwork(), this).disconnect();
}
