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

WiredPage::WiredPage(const QModelIndex &technology, Manager *manager, QWidget *parent):
    QWidget(parent),
    m_technology(technology)
{
    ui.setupUi(this);

    m_wiredTechnology = static_cast<ManagerNode*>(technology.internalPointer())->object<Technology *>();

    connect(manager, SIGNAL(servicesChanged()), SLOT(setIPV4WidgetService()));;
    connect(m_wiredTechnology, SIGNAL(poweredChanged()), SLOT(updateUi()));;
    connect(ui.enabled, SIGNAL(toggled(bool)), SLOT(toggleTechnology(bool)));

    updateUi();
}

void WiredPage::updateUi()
{
    ui.enabled->setChecked(m_wiredTechnology->isPowered());
}

void WiredPage::setIPV4WidgetService()
{
    ManagerNode *node = static_cast<ManagerNode*>(m_technology.child(0, 0).internalPointer()); // First ethernet service
    if (node && !ui.ipv4Widget->service())
        ui.ipv4Widget->setService(node->object<Service *>());
    else if (!node)
        ui.ipv4Widget->setService(NULL);
}

void WiredPage::toggleTechnology(bool enable)
{
    m_wiredTechnology->setPowered(enable);
}
