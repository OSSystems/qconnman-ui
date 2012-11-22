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
#include "ipv4configurationdialog.h"
#include "connman.h"

#include <qconnman/manager.h>
#include <qconnman/technology.h>
#include <qconnman/service.h>

#include <QTimer>
#include <QDebug>

static bool firstEth(Service *a, Service *b)
{
    return a->ethernet()->interface() < b->ethernet()->interface();
}

WiredPage::WiredPage(const QModelIndex &technology, Manager *manager, QWidget *parent):
    QWidget(parent),
    m_manager(manager),
    m_service(NULL)
{
    ui.setupUi(this);

    m_wiredTechnology = static_cast<ManagerNode*>(technology.internalPointer())->object<Technology *>();

    connect(manager, SIGNAL(servicesChanged()), SLOT(configureService()));;
    connect(m_wiredTechnology, SIGNAL(poweredChanged()), SLOT(updateUi()));;
    connect(ui.enabled, SIGNAL(toggled(bool)), SLOT(toggleTechnology(bool)));

    ui.ipv4Widget->hide();

    configureService();
    serviceStateChanged();
    updateUi();
}

void WiredPage::updateUi()
{
    ui.enabled->setChecked(m_wiredTechnology->isPowered());
    ui.advancedButton->setEnabled(m_wiredTechnology->isPowered());
}

void WiredPage::configureService()
{
    QList<Service *> ethernetServices;
    foreach (Service *service, m_manager->services())
    {
        if (service->type() != "ethernet") continue;
        if (!service->ethernet()->interface().startsWith("eth")) continue;
        ethernetServices << service;
    }

    qSort(ethernetServices.begin(), ethernetServices.end(), firstEth);

    if (ethernetServices.isEmpty())
    {
        if (m_wiredTechnology->isPowered())
            ui.status->setText(trUtf8("Cable unplugged"));
        ui.advancedButton->setEnabled(false);
        m_service = NULL;
    }
    else
    {
        m_service = ethernetServices.first();
        connect(m_service, SIGNAL(stateChanged()), SLOT(serviceStateChanged()));
        m_service->setAutoConnect(true);
        m_service->connect();
    }

    if (m_service)
        ui.ipv4Widget->setService(m_service);
    else if (!m_service)
        ui.ipv4Widget->setService(NULL);
}

void WiredPage::serviceStateChanged()
{
    if (!m_service)
    {
        ui.status->setText(trUtf8("Disconnected"));
        return;
    }

    if (m_service->state() == Service::IdleState)
    {
        ui.status->setText(trUtf8("Disconnected"));
        ui.ipv4Widget->hide();
    }
    else if (m_service->state() ==  Service::ReadyState || m_service->state() == Service::OnlineState)
    {
        ui.status->setText(trUtf8("Connected"));
        ui.ipv4Widget->unhide();
    }
    else if (m_service->state() == Service::ConfigurationState)
    {
        ui.advancedButton->setEnabled(true);
        ui.status->setText(trUtf8("Connecting"));
    }
}

void WiredPage::toggleTechnology(bool enable)
{
    m_wiredTechnology->setPowered(enable);
}

void WiredPage::on_advancedButton_clicked()
{
    Ipv4ConfigurationDialog dialog(m_service, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        dialog.applyConfiguration();

        m_service->disconnect();
        m_service->connect();
    }
}

