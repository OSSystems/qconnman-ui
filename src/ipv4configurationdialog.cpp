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

#include "ipv4configurationdialog.h"

#include <QDebug>

#include <qconnman/service.h>

Ipv4ConfigurationDialog::Ipv4ConfigurationDialog(Service *service, QWidget *parent):
    QDialog(parent),
    m_service(service)
{
    ui.setupUi(this);

    IPV4Data *ipv4 = service->ipv4Configuration();

    ui.autoConnect->setChecked(service->isAutoConnect());
    ui.autoIpAddress->setChecked(ipv4->method() == "dhcp");
    ui.manualIpAddress->setChecked(ipv4->method() != "dhcp");

    ui.ipAddress->setText(ipv4->address());
    ui.subnetMask->setText(ipv4->netmask());
    if (ipv4->gateway() != "0.0.0.0")
        ui.gateway->setText(ipv4->gateway());

    QStringList dnsList = service->nameserversConfiguration();
    if (dnsList.size() >= 1) ui.preferredDns->setText(dnsList.at(0));
    if (dnsList.size() >= 2) ui.alternateDns->setText(dnsList.at(1));
    if (!dnsList.isEmpty()) ui.manualDns->setChecked(true);
}

void Ipv4ConfigurationDialog::setService(Service *service)
{
}

void Ipv4ConfigurationDialog::applyConfiguration()
{
    m_service->setAutoConnect(ui.autoConnect->isChecked());

    if (ui.autoIpAddress->isChecked())
    {
        m_service->ipv4Configuration()->setMethod("dhcp");
    }
    else if (ui.manualIpAddress->isChecked())
    {
        m_service->ipv4Configuration()->setMethod("manual");
        m_service->ipv4Configuration()->setAddress(ui.ipAddress->text());
        m_service->ipv4Configuration()->setNetmask(ui.subnetMask->text());
        if (ui.gateway->text().isEmpty())
            m_service->ipv4Configuration()->setGateway("0.0.0.0");
        else
            m_service->ipv4Configuration()->setGateway(ui.gateway->text());
    }

    m_service->setNameserversConfiguration(QStringList() << ui.preferredDns->text() << ui.alternateDns->text());

    m_service->ipv4Configuration()->apply();
}

void Ipv4ConfigurationDialog::on_autoIpAddress_stateChanged(int state)
{
    ui.manualIpAddress->setChecked(state == Qt::Checked ? false : true);
}

void Ipv4ConfigurationDialog::on_manualIpAddress_toggled(bool on)
{
    ui.autoIpAddress->setCheckState(on ? Qt::Unchecked : Qt::Checked);
    if (on)
        ui.manualDns->setChecked(true);
}

void Ipv4ConfigurationDialog::on_autoDns_stateChanged(int state)
{
    ui.manualDns->setChecked(state == Qt::Checked ? false : true);

    if (state == Qt::Checked)
    {
        ui.autoIpAddress->setCheckState(Qt::Checked);
        ui.preferredDns->clear();
        ui.alternateDns->clear();
    }
}

void Ipv4ConfigurationDialog::on_manualDns_toggled(bool on)
{
    ui.autoDns->setCheckState(on ? Qt::Unchecked : Qt::Checked);
}
