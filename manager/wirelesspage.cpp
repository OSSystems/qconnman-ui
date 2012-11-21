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

#include "wirelesspage.h"
#include "connman.h"
#include "authdialog.h"
#include "hiddennetworkdialog.h"

#include <qconnman/manager.h>
#include <qconnman/service.h>

#include <QListView>
#include <QMessageBox>
#include <QDebug>
#include <QTreeView>

WirelessPage::WirelessPage(const QModelIndex &technology, Manager *manager, QWidget *parent):
    QWidget(parent)
{
    ui.setupUi(this);

    ui.listView->setModel(manager);
    ui.listView->setRootIndex(technology);
}
