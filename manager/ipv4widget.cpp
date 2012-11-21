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

#include "ipv4widget.h"
#include "widgetfilter.h"

#include <QDebug>

#include <qconnman/service.h>

Ipv4Widget::Ipv4Widget(QWidget *parent):
    QGroupBox(parent),
    m_service(NULL)
{
    ui.setupUi(this);
}

void Ipv4Widget::setService(Service *service)
{
    m_service = service;

    if (!service)
    {
        ui.hardwareAddress->clear();
        ui.ipAddress->clear();
        ui.subnetMask->clear();
        ui.gateway->clear();
        ui.dns->clear();
        return;
    }

    IPV4Data *ipv4 = service->ipv4();

    ui.hardwareAddress->setText(service->ethernet()->address());
    ui.ipAddress->setText(ipv4->address());
    ui.subnetMask->setText(ipv4->netmask());
    ui.gateway->setText(ipv4->gateway());
    if (!service->nameservers().isEmpty()) ui.dns->setText(service->nameservers().first());
}

void Ipv4Widget::hide()
{
    WidgetFilter::instance()->hide(this);
}

void Ipv4Widget::unhide()
{
    WidgetFilter::instance()->unhide(this);
}
