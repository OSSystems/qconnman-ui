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

#include "networkitemwidget.h"
#include "service.h"

#include <QListWidget>
#include <QDebug>

NetworkItemWidget::NetworkItemWidget(const QString &service, QListWidget *parent):
    QWidget(parent)
{
    ui.setupUi(this);

    m_item = new QListWidgetItem(parent);
    parent->setItemWidget(m_item, this);
    m_item->setSizeHint(sizeHint());
    m_item->setData(Qt::UserRole, service);

    m_service = new Service(service, this);

    update();
    toggleSpinner();

    connect(m_service, SIGNAL(stateChanged()), SLOT(toggleSpinner()));
    connect(m_service, SIGNAL(propertiesChanged()), SLOT(update()));
}

void NetworkItemWidget::update()
{
    if (!m_service->isHidden())
        ui.ssid->setText(m_service->name());
    else
        ui.ssid->setText("Hidden wireless network");
    ui.secured->setVisible(!m_service->security().contains("none"));
    ui.unsecured->setVisible(m_service->security().contains("none"));
    ui.securityIcon->setVisible(!m_service->security().contains("none"));
    ui.secured->setText(QString("Security-enabled network (%1)").arg(m_service->security().join(" ").toUpper()));

    ui.strength->setValue(m_service->strength());
}

void NetworkItemWidget::toggleSpinner()
{
    QString state = m_service->state();
    if ((QStringList() << "ready" << "online" << "failure" << "idle").contains(state))
        ui.spinner->setVisible(false);
    else if ((QStringList() << "association" << "configuration").contains(state))
        ui.spinner->setVisible(true);
}

