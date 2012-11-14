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

#include "networklistwidget.h"
#include "networkitemwidget.h"
#include "connman.h"

#include <qconnman/manager.h>
#include <qconnman/service.h>

#include <QTimer>
#include <QDebug>

NetworkListWidget::NetworkListWidget(QWidget *parent):
    QListWidget(parent)
{
    updateItems();
    connect(Connman::instance(), SIGNAL(servicesChanged()), SLOT(updateItems()));

    QTimer *timer = new QTimer(this);
    timer->start(30000);
    connect(timer, SIGNAL(timeout()), SLOT(scan()));
    scan();
}

QString NetworkListWidget::selectedNetwork() const
{
    QListWidgetItem *item = currentItem();
    if (!item)
        return QString();
    return item->data(Qt::UserRole).toString();
}

void NetworkListWidget::updateItems()
{
    QStringList services;
    foreach (Service *service, Connman::instance()->manager()->services())
        services.append(service->objectPath().path());

    foreach (const QString &service, services)
    {
        if (Connman::instance()->manager()->service(service)->type() != "wifi")
            continue;

        bool skip = false;

        for (int i = 0; i < count(); ++i)
        {
            QListWidgetItem *item = this->item(i);
            if (item->data(Qt::UserRole).toString() == service)
                skip = true;
        }

        if (skip)
            continue;

        new NetworkItemWidget(service, this);
    }

    QList<int> toRemove;
    for (int i = 0; i < count(); ++i)
    {
        QListWidgetItem *item = this->item(i);
        if (!services.contains(item->data(Qt::UserRole).toString()))
            toRemove.append(i);
    }

    foreach (int i, toRemove)
        takeItem(i);
}

void NetworkListWidget::scan()
{
    qDebug("RequestScan");
//    Connman::instance()->requestScan("wifi");
}

