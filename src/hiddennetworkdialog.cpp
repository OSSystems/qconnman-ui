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

#include <qconnman/agent.h>

HiddenNetworkDialog::HiddenNetworkDialog(QWidget *parent):
    QDialog(parent)
{
    ui.setupUi(this);

    ui.icon->setPixmap(QIcon::fromTheme("network-wireless").pixmap(QSize(48, 48)));
}

int HiddenNetworkDialog::exec()
{
    Agent *agent = qobject_cast<Agent *>(QObject::sender());
    Agent::InputRequest *request = agent->currentInputRequest();

    ui.name->clear();
    ui.name->setFocus();

    int result = QDialog::exec();
    if (result == QDialog::Accepted)
        request->response.name = ui.name->text();
    else
        request->cancel = true;

    return result;
}
