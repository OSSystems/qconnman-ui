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

#include "technologyitemwidget.h"

#include <QListWidget>
#include <QDebug>

TechnologyItemWidget::TechnologyItemWidget(const QString &technology, QListWidget *parent):
    QWidget(parent)
{
    ui.setupUi(this);

    if (technology == "ethernet")
    {
        ui.icon->setPixmap(QPixmap(":/images/network-wired.png"));
        ui.label->setText("Wired");
    }
    else if (technology == "wifi")
    {
        ui.icon->setPixmap(QPixmap(":/images/network-wireless.png"));
        ui.label->setText("Wireless");
    }

    m_item = new QListWidgetItem(parent);
    parent->setItemWidget(m_item, this);
    m_item->setSizeHint(sizeHint());
    m_item->setData(Qt::UserRole, technology);
}
