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

#include <QTimer>
#include <QDebug>

static bool sorta(Service *a, Service *b)
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

    connect(manager, SIGNAL(servicesChanged()), SLOT(setService()));;
    connect(m_wiredTechnology, SIGNAL(poweredChanged()), SLOT(updateUi()));;
    connect(ui.enabled, SIGNAL(toggled(bool)), SLOT(toggleTechnology(bool)));

    setService();
    updateUi();
}

void WiredPage::updateUi()
{
    ui.enabled->setChecked(m_wiredTechnology->isPowered());
    ui.advancedButton->setEnabled(m_wiredTechnology->isPowered());
    if (m_wiredTechnology->isPowered())
        ui.ipv4Widget->unhide();
    else
        ui.ipv4Widget->hide();
}

void WiredPage::setService()
{
    QList<Service *> ethernetServices;
    foreach (Service *service, m_manager->services())
    {
        if (service->type() != "ethernet") continue;
        if (!service->ethernet()->interface().startsWith("eth")) continue;
        ethernetServices << service;
    }

    qSort(ethernetServices.begin(), ethernetServices.end(), sorta);

    if (ethernetServices.isEmpty())
        m_service = NULL;
    else
    {
        m_service = ethernetServices.first();
        m_service->connect();
    }

    if (m_service)
        ui.ipv4Widget->setService(m_service);
    else if (!m_service)
        ui.ipv4Widget->setService(NULL);
}

void WiredPage::toggleTechnology(bool enable)
{
    m_wiredTechnology->setPowered(enable);
    ui.advancedButton->setEnabled(enable);
    if (enable)
        ui.ipv4Widget->unhide();
    else
        ui.ipv4Widget->hide();
}
