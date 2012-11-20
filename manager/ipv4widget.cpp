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

#include <QDebug>

#include <qconnman/service.h>

Ipv4Widget::Ipv4Widget(QWidget *parent):
    QFrame(parent)
{
    ui.setupUi(this);
}

void Ipv4Widget::setSettings(IPV4Data *ipv4)
{
    if (!ipv4)
    {
        ui.autoIpAddress->setChecked(true);
        ui.autoDns->setChecked(true);

        ui.address->clear();
        ui.netmask->clear();
        ui.gateway->clear();

        return;
    }

    ui.autoIpAddress->setChecked(ipv4->method() == "dhcp");
    ui.manualIpAddress->setChecked(ipv4->method() != "dhcp");

    ui.address->setText(ipv4->address());
    ui.netmask->setText(ipv4->netmask());
    if (ipv4->gateway() != "0.0.0.0")
        ui.gateway->setText(ipv4->gateway());
}

void Ipv4Widget::apply(Service *service)
{
    if (ui.autoIpAddress->isChecked())
    {
        service->ipv4Configuration()->setMethod("dhcp");
    }
    else if (ui.manualIpAddress->isChecked())
    {
        service->ipv4Configuration()->setMethod("manual");
        service->ipv4Configuration()->setAddress(ui.address->text());
        service->ipv4Configuration()->setNetmask(ui.netmask->text());
        if (ui.gateway->text().isEmpty())
            service->ipv4Configuration()->setGateway("0.0.0.0");
        else
            service->ipv4Configuration()->setGateway(ui.gateway->text());

        service->ipv4Configuration()->apply();
    }
}

void Ipv4Widget::on_autoIpAddress_stateChanged(int state)
{
    ui.manualIpAddress->setChecked(state == Qt::Checked ? false : true);
}

void Ipv4Widget::on_manualIpAddress_toggled(bool on)
{
    ui.autoIpAddress->setCheckState(on ? Qt::Unchecked : Qt::Checked);
    if (on)
        ui.manualDns->setChecked(true);
}

void Ipv4Widget::on_autoDns_stateChanged(int state)
{
    ui.manualDns->setChecked(state == Qt::Checked ? false : true);

    if (state == Qt::Checked)
    {
        ui.autoIpAddress->setCheckState(Qt::Checked);
        ui.preferredDns->clear();
        ui.alternateDns->clear();
    }
}

void Ipv4Widget::on_manualDns_toggled(bool on)
{
    ui.autoDns->setCheckState(on ? Qt::Unchecked : Qt::Checked);
}
