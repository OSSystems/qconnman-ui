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
#include "authdialog.h"
#include "hiddennetworkinputdialog.h"
#include "connman.h"

#include <qconnman/agent.h>
#include <qconnman/managerinterface.h>
#include <qconnman/manager_p.h>

#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent):
    QWidget(parent)
{
    ui.setupUi(this);

    m_manager = new ConnMan(this);
//    connect(m_manager, SIGNAL(rowsInserted(QModelIndex, int ,int)),
//            SLOT(createTechnologyItemWidgets(QModelIndex, int, int)));
    ui.technologyListView->setModel(m_manager);
    ui.technologyListView->setIconSize(QSize(32, 32));

    connect(ui.technologyListView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            SLOT(changePage(const QModelIndex &, const QModelIndex &)));

    ui.technologyListView->selectionModel()->select(m_manager->index(0, 0), QItemSelectionModel::SelectCurrent);

    // create initial widgets
    //createTechnologyItemWidgets(QModelIndex(), 0, m_manager->rowCount());

    m_agent = new Agent("/com/OSSystems/ConnMan", m_manager);
    connect(m_agent, SIGNAL(passphraseRequested()), (new AuthDialog(m_manager, this)), SLOT(exec()));
    connect(m_agent, SIGNAL(nameRequested()), (new HiddenNetworkInputDialog(this)), SLOT(exec()));
    connect(m_agent, SIGNAL(errorRaised()), SLOT(reportError()));
}

void MainWindow::changePage(const QModelIndex &technology, const QModelIndex &previous)
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
            page = new WiredPage(technology, m_manager, this);
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

void MainWindow::reportError()
{
    Agent::ErrorRequest *request = m_agent->currentErrorRequest();
    if (request->error == "invalid-key")
    {
        QMessageBox::StandardButton result = QMessageBox::critical(this, trUtf8("Invalid password"),
                                                                   trUtf8("Unable to connect to '%1' wireless network because the entered password is invalid.").
                                                                   arg(m_manager->service(request->service)->name()),
                                                                   QMessageBox::Retry | QMessageBox::Cancel, QMessageBox::Retry);
        request->retry = result == (QMessageBox::Retry);
    }

    qDebug() << request->error;
}
