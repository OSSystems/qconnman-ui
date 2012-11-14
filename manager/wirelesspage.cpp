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
#include "authdialog.h"
#include "hiddennetworkdialog.h"

#include <qconnman/manager.h>
#include <qconnman/service.h>

#include <QMessageBox>
#include <QDebug>

WirelessPage::WirelessPage(QWidget *parent):
    QWidget(parent)
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

Service *WirelessPage::connectedServicePath()
{
    Q_FOREACH (Service *service, Connman::instance()->manager()->services())
    {
        if ((service->state() == Service::ReadyState || service->state() == Service::OnlineState) && service->type() == "wifi")
            return service;
    }
    return NULL;
}

void WirelessPage::updateState()
{
    Technology *wifi = NULL;
    foreach (Technology *technology, Connman::instance()->manager()->technologies())
        if (technology->name() == "wifi")
            wifi = technology;

    bool enabled = wifi->isPowered();
    bool connected = wifi->isConnected();

    ui.enableCheck->setChecked(enabled);
    ui.networkList->setEnabled(enabled);

    if (!enabled)
    {
        ui.status->setText(tr("Disabled"));
        ui.info->setText("");
    }

    if (connected)
    {
        ui.status->setText(tr("Connected"));

        foreach (Service *service, Connman::instance()->manager()->services())
        {
            if ((service->state() == Service::ReadyState || service->state() == Service::OnlineState) && service->type() == "wifi")
            {
                ui.info->setText(tr("Wireless device is connected to '%1' and has the IP Address %2")
                                 .arg(service->name())
                                 .arg(service->ipv4()->address()));
                break;
            }
        }
    }
    else if (enabled && !connected)
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

    ui.connectButton->setEnabled(connectedServicePath()->objectPath().path() != ui.networkList->selectedNetwork());
    ui.disconnectButton->setEnabled(connectedServicePath()->objectPath().path() == ui.networkList->selectedNetwork());
}

void WirelessPage::networkListItemChanged(int row)
{
    if (!connectedServicePath())
    {
        ui.connectButton->setEnabled(true);
        ui.disconnectButton->setEnabled(false);
        return;
    }

    ui.connectButton->setEnabled(connectedServicePath()->objectPath().path() != ui.networkList->selectedNetwork());
    ui.disconnectButton->setEnabled(connectedServicePath()->objectPath().path() == ui.networkList->selectedNetwork());
}

void WirelessPage::toggleTechnology(bool checked)
{
    Technology *wifi = NULL;
    foreach (Technology *technology, Connman::instance()->manager()->technologies())
        if (technology->name() == "wifi")
            wifi = technology;

    wifi->setPowered(checked);
}

void WirelessPage::connectToNetwork()
{
/*    foreach (Service *service, Connman::instance()->manager()->services())
    {
        if (service->type() != "wifi")
            continue;

        if (service->state() != Service::AssociationState || service->state() != Service::ConfigurationState)
            continue;

        if (service->objectPath().path() != ui.networkList->selectedNetwork())
            QMessageBox::warning(this, tr("Unable to connect"), tr("Can't connect to this network while another connection is being established."));

        return;
    }

    Service service(ui.networkList->selectedNetwork(), this);
    service.setAutoConnect(true);
    service.connect();*/

    foreach (Service *service, Connman::instance()->manager()->services())
    {
        if (service->objectPath().path() == ui.networkList->selectedNetwork())
        {
            service->setAutoConnect(true);
            service->connect();
            break;
        }
    }
}

void WirelessPage::disconnect()
{
    foreach (Service *service, Connman::instance()->manager()->services())
    {
        if (service->objectPath().path() == ui.networkList->selectedNetwork())
        {
            service->disconnect();
            break;
        }
    }
}
