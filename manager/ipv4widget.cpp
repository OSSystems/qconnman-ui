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

    if (service) connect(service, SIGNAL(dataChanged()), SLOT(updateUi()));

    updateUi();
}

void Ipv4Widget::hide()
{
    WidgetFilter::instance()->hide(this);
}

void Ipv4Widget::unhide()
{
    WidgetFilter::instance()->unhide(this);
}

void Ipv4Widget::updateUi()
{
    if (!m_service)
    {
        ui.hardwareAddress->clear();
        ui.ipAddress->clear();
        ui.subnetMask->clear();
        ui.gateway->clear();
        ui.dns->clear();
        return;
    }

    IPV4Data *ipv4 = m_service->ipv4();

    if (ipv4->gateway().isEmpty())
    {
        ui.gatewayLabel->hide();
        ui.gateway->hide();
    }

    if (m_service->nameservers().isEmpty())
    {
        ui.dnsLabel->hide();
        ui.dns->hide();
    }

    ui.hardwareAddress->setText(m_service->ethernet()->address());
    ui.ipAddress->setText(ipv4->address());
    ui.subnetMask->setText(ipv4->netmask());
    ui.gateway->setText(ipv4->gateway());
    if (!m_service->nameservers().isEmpty()) ui.dns->setText(m_service->nameservers().first());
}
