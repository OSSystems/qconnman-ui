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

#ifndef _WIREDPAGE_H
#define _WIREDPAGE_H

#include "ui_wiredpage.h"

class Technology;
class Manager;

class WiredPage: public QWidget
{
    Q_OBJECT

public:
    WiredPage(const QModelIndex &technology, Manager *manager, QWidget *parent);

private:
    QList<Service *> wiredServices();

private slots:
    void updateUi();
    void configureService();
    void toggleTechnology(bool enable);

    void on_advancedButton_clicked();

private:
    Ui::WiredPage ui;
    QModelIndex m_technology;
    Technology *m_wiredTechnology;
    Manager *m_manager;
    Service *m_service;
};

#endif
