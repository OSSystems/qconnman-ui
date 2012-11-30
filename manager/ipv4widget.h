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

#ifndef _IPV4WIDGET_H
#define _IPV4WIDGET_H

#include "ui_ipv4widget.h"

class Service;

class Ipv4Widget: public QGroupBox
{
    Q_OBJECT

public:
    Ipv4Widget(QWidget *parent);

    inline Service *service() const { return m_service; }

    void setService(Service *service);

    void hide();
    void unhide();

private slots:
    void updateUi();
    void unconfigureService();

private:
    Ui::Ipv4Widget ui;
    Service *m_service;
};

#endif
