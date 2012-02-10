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
#include "service.h"
#include "technology.h"

#include <QDebug>

WiredPage::WiredPage(const QString &technology, QWidget *parent):
    QWidget(parent),
    m_technology(technology)
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
    bool enabled = Connman::instance()->isTechnologyEnabled(m_technology);
    bool connected = Connman::instance()->isTechnologyConnected(m_technology);

    ui.enableCheck->setChecked(enabled);

    ui.connectButton->setEnabled(enabled);
    ui.disconnectButton->setEnabled(enabled);
    ui.configFrame->setEnabled(enabled && !connected);

    if (!enabled)
    {
        ui.status->setText(tr("Disabled"));
        ui.info->setText("");
    }

    QString state = Technology(Connman::instance()->technologyPath("ethernet"), this).state();
    if (state == "connected" || connected)
    {
        ui.connectButton->setEnabled(false);
        ui.disconnectButton->setEnabled(true);
        ui.status->setText(tr("Connected"));
        ui.info->setText(tr("Wired device is connected and has the IP Address %1")
                         .arg(Service(Connman::instance()->ethernetService(), this).ipv4Settings()["Address"].toString()));
    }
    else if (enabled && (state == "offline" || !connected))
    {
        ui.connectButton->setEnabled(true);
        ui.disconnectButton->setEnabled(false);
        ui.status->setText(tr("Disconnected"));
        ui.info->setText("");
    }

    QString servicePath = Connman::instance()->ethernetService();
    if (!servicePath.isEmpty())
        ui.ipv4Widget->setSettings(Service(servicePath, this).ipv4Settings());
}

void WiredPage::toggleTechnology(bool checked)
{
    if (checked)
        Connman::instance()->enableTechnology(m_technology);
    else
        Connman::instance()->disableTechnology(m_technology);
}

void WiredPage::connect()
{
    QString servicePath = Connman::instance()->ethernetService();
    if (servicePath.isEmpty())
        return;

    Service service(servicePath, this);
    service.setIpv4Settings(ui.ipv4Widget->toMap());
    service.connect();
}

void WiredPage::disconnect()
{
    QString service = Connman::instance()->ethernetService();
    if (service.isEmpty())
        return;

    Service(service, this).disconnect();
}
