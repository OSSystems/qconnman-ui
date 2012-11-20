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

#include "technologylistwidget.h"
#include "technologyitemwidget.h"
#include "connman.h"

#include <qconnman/manager.h>

#include <QDebug>

TechnologyListWidget::TechnologyListWidget(QWidget *parent):
    QListWidget(parent)
{
    connect(this, SIGNAL(currentRowChanged(int)), SLOT(emitItemChanged(int)));
    connect(Connman::instance(), SIGNAL(technologiesChanged()), SLOT(updateItems()));

    updateItems();
}

void TechnologyListWidget::updateItems()
{
    QStringList technologies;
    foreach (Technology *technology, Connman::instance()->manager()->technologies())
        technologies.append(technology->name());

    foreach (const QString &technology, technologies)
    {
        bool skip = false;

        for (int i = 0; i < count(); ++i)
        {
            QListWidgetItem *item = this->item(i);
            if (item->data(Qt::UserRole).toString() == technology)
                skip = true;
        }

        if (skip)
            continue;

        new TechnologyItemWidget(technology, this);
    }

    QList<int> toRemove;
    for (int i = 0; i < count(); ++i)
    {
        QListWidgetItem *item = this->item(i);
        if (!technologies.contains(item->data(Qt::UserRole).toString()))
            toRemove.append(i);
    }

    foreach (int i, toRemove)
        takeItem(i);
}

void TechnologyListWidget::emitItemChanged(int row)
{
    QListWidgetItem *item = this->item(row);
    emit itemChanged(item->data(Qt::UserRole).toString());
}
