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

#ifndef _WIRELESSPAGE_H
#define _WIRELESSPAGE_

#include "ui_wirelesspage.h"

class Technology;

class WirelessPage: public QWidget
{
    Q_OBJECT

public:
    WirelessPage(QWidget *parent);

private:
    QString connectedServicePath();

private slots:
    void updateState();

    void networkListItemChanged(int row);

    void toggleTechnology(bool checked);

    void connectToNetwork();
    void disconnect();

    void showAddNetworkDialog();

private:
    Ui::WirelessPage ui;
    Technology *m_technology;
};

#endif
