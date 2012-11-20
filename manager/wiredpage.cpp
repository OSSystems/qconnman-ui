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

#include "wiredpage.h"
#include "connman.h"

#include <qconnman/manager.h>
#include <qconnman/technology.h>
#include <qconnman/service.h>

#include <QDebug>

WiredPage::WiredPage(QWidget *parent):
    QWidget(parent)
{
    ui.setupUi(this);

    updateButtonsVisibility();

    QObject::connect(ui.enableCheck, SIGNAL(clicked(bool)), SLOT(toggleTechnology(bool)));

    QObject::connect(ui.connectButton, SIGNAL(clicked()), SLOT(connect()));
    QObject::connect(ui.disconnectButton, SIGNAL(clicked()), SLOT(disconnect()));

    QObject::connect(Connman::instance(), SIGNAL(enabledTechnologiesChanged()), SLOT(updateButtonsVisibility()));
    QObject::connect(Connman::instance(), SIGNAL(connectedTechnologiesChanged()), SLOT(updateButtonsVisibility()));
}

void WiredPage::updateButtonsVisibility()
{
    Technology *wired = NULL;
    foreach (Technology *technology, Connman::instance()->manager()->technologies())
        if (technology->name() == "Wired")
            wired = technology;

    bool enabled = wired->isPowered();
    bool connected = wired->isConnected();

    ui.enableCheck->setChecked(enabled);

    ui.connectButton->setEnabled(enabled);
    ui.disconnectButton->setEnabled(enabled);
    ui.configFrame->setEnabled(enabled && !connected);

    if (!enabled)
    {
        ui.status->setText(tr("Disabled"));
        ui.info->setText("");
    }

    if (connected)
    {
        ui.connectButton->setEnabled(false);
        ui.disconnectButton->setEnabled(true);
        ui.status->setText(tr("Connected"));

        foreach (Service *service, Connman::instance()->manager()->services())
        {
            if ((service->state() == Service::ReadyState || service->state() == Service::OnlineState) && service->type() == "Wired")
            {
                ui.info->setText(tr("Wired device is connected and has the IP Address %1")
                                 .arg(service->ipv4()->address()));
                ui.ipv4Widget->setSettings(service->ipv4());
                break;
            }
        }
    }
    else if (enabled && !connected)
    {
        ui.connectButton->setEnabled(true);
        ui.disconnectButton->setEnabled(false);
        ui.status->setText(tr("Disconnected"));
        ui.info->setText("");
    }
}

void WiredPage::toggleTechnology(bool checked)
{
    Technology *wired = NULL;
    foreach (Technology *technology, Connman::instance()->manager()->technologies())
        if (technology->name() == "Wired")
            wired = technology;

    wired->setPowered(checked);
}

void WiredPage::connect()
{
/*    QString servicePath = Connman::instance()->ethernetService();
    if (servicePath.isEmpty())
        return;

    Service service(servicePath, this);
    service.setIpv4Settings(ui.ipv4Widget->toMap());
    service.connect();*/

    foreach (Service *service, Connman::instance()->manager()->services())
    {
        if (service->type() == "ethernet")
        {
            ui.ipv4Widget->apply(service);
            service->setAutoConnect(true);
            service->connect();
            break;
        }
    }
}

void WiredPage::disconnect()
{
    foreach (Service *service, Connman::instance()->manager()->services())
    {
        if (service->type() == "ethernet")
        {
            service->disconnect();
            break;
        }
    }
}
