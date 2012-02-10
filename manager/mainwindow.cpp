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

#include "mainwindow.h"
#include "wiredpage.h"
#include "wirelesspage.h"
#include "agent.h"
#include "connman.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent):
    QWidget(parent)
{
    ui.setupUi(this);

    new Agent(this);
    Connman::instance()->registerAgent("/");

    connect(ui.technologyListWidget, SIGNAL(itemChanged(const QString &)), SLOT(changePage(const QString &)));
}

void MainWindow::changePage(const QString &technology)
{
    QWidget *widget = m_pages.value(technology);
    if (!widget)
    {
        if (technology.endsWith("ethernet"))
        {
            widget = new WiredPage(technology, this);
            ui.stackedWidget->addWidget(widget);
            m_pages.insert(technology, widget);
        }
        else if (technology.endsWith("wifi"))
        {
            widget = new WirelessPage(technology, this);
            ui.stackedWidget->addWidget(widget);
            m_pages.insert(technology, widget);
        }
    }

    ui.stackedWidget->setCurrentWidget(widget);
}

