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

#include "hiddennetworkdialog.h"
#include "connman.h"

#include <qconnman/agent.h>

HiddenNetworkDialog::HiddenNetworkDialog(ConnMan *manager, QWidget *parent):
    QDialog(parent),
    m_manager(manager)
{
    ui.setupUi(this);

    ui.icon->setPixmap(QIcon::fromTheme("network-wireless").pixmap(QSize(48, 48)));

    foreach (Service *service, manager->services())
    {
        if (service->type() == "wifi" && service->name().isEmpty())
        {
            if (service->security().contains("none"))
                ui.networkList->addItem(trUtf8("Unsecured wireless network"), qVariantFromValue(service));
            else
                ui.networkList->addItem(trUtf8("Security-enabled wireless network (%1)").
                                        arg(service->security().join(" ").toUpper()), qVariantFromValue(service));
        }
    }
}

int HiddenNetworkDialog::exec()
{
    int result = QDialog::exec();
    if (result == QDialog::Accepted)
    {
        Service *service = qvariant_cast<Service *>(ui.networkList->itemData(ui.networkList->currentIndex()));
        service->connect();
    }

    return result;
}
