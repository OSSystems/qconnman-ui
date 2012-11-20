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
#include "technologyitemwidget.h"

#include <qconnman/manager.h>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent):
    QWidget(parent)
{
    ui.setupUi(this);

    m_manager = new Manager(this);
    connect(ui.technologyListView, SIGNAL(clicked(QModelIndex)), this, SLOT(changePage(QModelIndex)));
//    connect(m_manager, SIGNAL(rowsInserted(QModelIndex, int ,int)),
//            SLOT(createTechnologyItemWidgets(QModelIndex, int, int)));
    ui.technologyListView->setModel(m_manager);
    ui.technologyListView->setIconSize(QSize(32, 32));
    // create initial widgets
    //createTechnologyItemWidgets(QModelIndex(), 0, m_manager->rowCount());
}

void MainWindow::changePage(const QModelIndex &technology)
{
    ManagerNode *node = static_cast<ManagerNode*>(technology.internalPointer());
    if (!node || !node->isTechnology()) {
        qDebug() << "something really bad happened";
        return;
    }

    QWidget *page = m_pages.value(technology);
    if (!page)
    {
        Technology *technologyObject = node->object<Technology*>();
        QString technologyType = technologyObject->type().toLower();
        if (technologyType == "ethernet")
            page = new WiredPage(technology, this);
        else if (technologyType == "wifi")
            page = new WirelessPage(technology, m_manager, this);
        else
        {
            qDebug() << "unsupported technology type: " << technologyType;
            return;
        }

        ui.stackedWidget->addWidget(page);
        m_pages.insert(technology, page);
    }

    ui.stackedWidget->setCurrentWidget(page);
}

void MainWindow::createTechnologyItemWidgets(const QModelIndex &parent, int start, int end)
{
    for (int row = start; row < end; ++row) {
        QModelIndex index = m_manager->index(row, 0, parent);
        ManagerNode *node = static_cast<ManagerNode*>(index.internalPointer());
        if (node->isTechnology()) {
            Technology *technology = node->object<Technology*>();
            ui.technologyListView->setIndexWidget(index, new TechnologyItemWidget(technology));
        }
    }
}
