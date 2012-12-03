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

#include <qconnman/manager.h>
#include <qconnman/technology.h>
#include <qconnman/service.h>

#include <QTimer>
#include <QDebug>

static bool ethernetSort(Service *a, Service *b)
{
    return a->ethernet()->interface() < b->ethernet()->interface();
}

WiredPage::WiredPage(const QModelIndex &technology, Manager *manager, QWidget *parent):
    QWidget(parent),
    m_technology(technology),
    m_manager(manager),
    m_service(NULL)
{
    ui.setupUi(this);

    m_wiredTechnology = static_cast<ManagerNode*>(technology.internalPointer())->object<Technology *>();

    ui.icon->setPixmap(QIcon::fromTheme("network-wired").pixmap(QSize(48, 48)));
    ui.enabled->setChecked(m_wiredTechnology->isPowered());

    connect(manager, SIGNAL(servicesChanged()), SLOT(configureService()));
    connect(m_wiredTechnology, SIGNAL(dataChanged()), SLOT(updateUi()));
    connect(m_wiredTechnology, SIGNAL(poweredChanged(bool)), ui.enabled, SLOT(setChecked(bool)));
    connect(ui.enabled, SIGNAL(toggled(bool)), SLOT(toggleTechnology(bool)));

    ui.ipv4Widget->hide();

    configureService();
    updateUi();
}

QList<Service *> WiredPage::wiredServices()
{
    QList<Service *> services;

    for (int i = 0; i < m_technology.model()->rowCount(m_technology); i++)
    {
        Service *service = static_cast<ManagerNode *>(m_technology.child(i, 0).internalPointer())->object<Service *>();
        if (!service->ethernet()->interface().startsWith("eth"))
        {
            service->setAutoConnect(false);
            continue;
        }
        services << service;
    }

    qSort(services.begin(), services.end(), ethernetSort);

    return services;
}

void WiredPage::updateUi()
{
    ui.advancedButton->setEnabled(m_service != NULL);

    if (m_service && m_wiredTechnology->isPowered() && (m_service->state() == Service::ReadyState || m_service->state() == Service::OnlineState))
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

void WiredPage::configureService()
{
    QList<Service *> services = wiredServices();
    if (!services.isEmpty() && m_service != services.first())
    {
        m_service = services.first();
        connect(m_service, SIGNAL(dataChanged()), SLOT(updateUi()));
        connect(m_service, SIGNAL(destroyed()), SLOT(unconfigureService()));

        updateUi();
    }
}

void WiredPage::unconfigureService()
{
    Service *service = (Service *)QObject::sender();
    if (!service)
        return;
    m_service = NULL;
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

        if (m_service->isAutoConnect())
        {
            m_service->disconnect();
            m_service->connect();
        }
        else
            m_service->disconnect();
    }
}
