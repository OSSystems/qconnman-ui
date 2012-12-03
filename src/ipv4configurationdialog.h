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

#ifndef _IPV4CONFIGURATIONDIALOG_H
#define _IPV4CONFIGURATIONDIALOG_H

#include "ui_ipv4configurationdialog.h"

class Service;

class Ipv4ConfigurationDialog: public QDialog
{
    Q_OBJECT

public:
    Ipv4ConfigurationDialog(Service *service, QWidget *parent);

    inline Service *service() const { return m_service; }

    void setService(Service *service);
    void applyConfiguration();

private slots:
    void on_autoIpAddress_stateChanged(int state);
    void on_manualIpAddress_toggled(bool on);
    void on_autoDns_stateChanged(int state);
    void on_manualDns_toggled(bool on);

private:
    Ui::Ipv4ConfigurationDialog ui;
    Service *m_service;
};

#endif
