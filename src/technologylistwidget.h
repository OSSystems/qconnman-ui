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

#ifndef _TECHNOLOGYLISTWIDGET_H
#define _TECHNOLOGYLISTWIDGET_H

#include <QListWidget>

class TechnologyListWidget: public QListWidget
{
    Q_OBJECT

public:
    TechnologyListWidget(QWidget *parent = 0);

signals:
    void itemChanged(const QString &technology);

private slots:
    void updateItems();
    void emitItemChanged(int row);
};

#endif

