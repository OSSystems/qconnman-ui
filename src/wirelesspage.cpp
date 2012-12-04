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

#include "wirelesspage.h"
#include "authdialog.h"
#include "ipv4configurationdialog.h"
#include "connman.h"

#include <qconnman/service.h>

#include <QListView>
#include <QMessageBox>
#include <QDebug>
#include <QTreeView>

WirelessPage::WirelessPage(const QModelIndex &technology, ConnMan *manager, QWidget *parent):
    QWidget(parent),
    m_technology(technology),
    m_service(NULL)
{
    ui.setupUi(this);

    m_wireless = static_cast<ManagerNode*>(technology.internalPointer())->object<Technology *>();

    ui.icon->setPixmap(QIcon::fromTheme("network-wireless").pixmap(QSize(48, 48)));
    ui.enabled->setChecked(m_wireless->isPowered());
    ui.advancedButton->setEnabled(false);
    ui.ipv4Widget->hide();

    ui.networkList->setModel(manager);
    ui.networkList->setRootModelIndex(technology);
    ui.networkList->setCurrentIndex(-1);

    connect(m_wireless, SIGNAL(poweredChanged(bool)), ui.enabled, SLOT(setChecked(bool)));
    connect(m_wireless, SIGNAL(poweredChanged(bool)), SLOT(updateUi()));
    connect(manager, SIGNAL(servicesChanged()), SLOT(configureService()));
    connect(m_wireless, SIGNAL(dataChanged()), SLOT(updateUi()));
    connect(ui.enabled, SIGNAL(toggled(bool)), SLOT(toggleTechnology(bool)));
    connect(ui.networkList, SIGNAL(currentIndexChanged(int)), SLOT(setService(int)));

    configureService();
    updateUi();
}

void WirelessPage::updateUi()
{
	ui.networkList->setEnabled(m_wireless->isPowered());
 	ui.networkNameLabel->setEnabled(m_wireless->isPowered());

    ui.enabled->setChecked(m_wireless->isPowered());

    if (m_service && m_wireless->isPowered() && (m_service->state() == Service::ReadyState || m_service->state() == Service::OnlineState))
    {
        ui.status->setText("Connected");
        ui.ipv4Widget->setService(m_service);
        ui.ipv4Widget->unhide();
        ui.advancedButton->setEnabled(true);
    }
    else if (!m_service || (m_service->state() == Service::IdleState || m_service->state() == Service::DisconnectState))
    {
        ui.status->setText("Disconnected");
        ui.ipv4Widget->hide();
    }
}

void WirelessPage::configureService()
{
    for (int i = 0; i < m_technology.model()->rowCount(m_technology); i++)
    {
        Service *service = static_cast<ManagerNode *>(m_technology.child(i, 0).internalPointer())->object<Service *>();
        if (service->type() != "wifi") continue;

        if (service->state() == Service::ReadyState || service->state() == Service::OnlineState)
        {
            ui.networkList->setCurrentIndex(i);
            m_service = service;
            updateUi();
            break;
        }
    }
}

void WirelessPage::unconfigureService()
{
    Service *service = (Service *)QObject::sender();
    if (!service)
        return;
    m_service = NULL;
}

void WirelessPage::toggleTechnology(bool enable)
{
    m_wireless->setPowered(enable);
    if (!enable)
        ui.networkList->setCurrentIndex(-1);
}

void WirelessPage::setService(int index)
{
	if (index == -1) 
    {
        ui.advancedButton->setEnabled(false);
        ui.clearButton->setEnabled(false);
        return;
    }

    ui.advancedButton->setEnabled(true);

    ManagerNode *node = static_cast<ManagerNode *>(m_technology.child(index, 1).internalPointer());
    m_service = node->object<Service *>();

    ui.clearButton->setEnabled(m_service->isFavorite());

    ui.ipv4Widget->setService(m_service);

    connect(m_service, SIGNAL(dataChanged()), SLOT(updateUi()));
    connect(m_service, SIGNAL(destroyed()), SLOT(unconfigureService()));

    if (m_service->state() == Service::ReadyState || m_service->state() == Service::OnlineState)
        ui.ipv4Widget->unhide();
    else
        ui.ipv4Widget->hide();

    m_service->connect();
}

void WirelessPage::on_advancedButton_clicked()
{
    Ipv4ConfigurationDialog dialog(m_service, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        dialog.applyConfiguration();

        if (m_service->isAutoConnect())
        {
            m_service->disconnect();
            m_service->connect();
        }
        else
            m_service->disconnect();
    }
}

void WirelessPage::on_clearButton_clicked()
{
    QMessageBox::StandardButton result = QMessageBox::question(this, trUtf8("Are you sure?"),
                                                               trUtf8("Are you sure want to delete details of this network including password and any other configuration?"),
                                                               QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes) m_service->remove();
}
