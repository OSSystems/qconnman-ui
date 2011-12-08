/*
 * qconnman - Connman Applet
 * Copyright (C) 2011 O.S. Systems
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
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

Ipv4Widget::Ipv4Widget(QWidget *parent):
    QFrame(parent)
{
    ui.setupUi(this);
}

void Ipv4Widget::setSettings(const QVariantMap &settings)
{
    if (settings.isEmpty())
    {
        ui.autoIpAddress->setChecked(true);
        ui.autoDns->setChecked(true);

        ui.address->clear();
        ui.netmask->clear();
        ui.gateway->clear();

        return;
    }

    ui.autoIpAddress->setChecked(settings["Method"] == "dhcp");
    ui.manualIpAddress->setChecked(settings["Method"] != "dhcp");

    ui.address->setText(settings["Address"].toString());
    ui.netmask->setText(settings["Netmask"].toString());
    if (settings["Gateway"].toString() != "0.0.0.0")
        ui.gateway->setText(settings["Gateway"].toString());
}

QVariantMap Ipv4Widget::toMap()
{
    QVariantMap map;

    if (ui.autoIpAddress->isChecked())
    {
        map["Method"] = "dhcp";
    }
    else if (ui.manualIpAddress->isChecked())
    {
        map["Method"] = "manual";
        map["Address"] = ui.address->text();
        map["Netmask"] = ui.netmask->text();
        if (ui.gateway->text().isEmpty())
            map["Gateway"] = "0.0.0.0";
        else
            map["Gateway"] = ui.gateway->text();
    }

    return map;
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
