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

HiddenNetworkDialog::HiddenNetworkDialog(QWidget *parent):
    QDialog(parent)
{
    ui.setupUi(this);

//    ui.label->setText(ui.label->text().arg(security));

    connect(ui.showPassword, SIGNAL(toggled(bool)), SLOT(showPassword(bool)));
}

QVariantMap HiddenNetworkDialog::toMap()
{
    QVariantMap map;
    map["Name"] = ui.networkName->text();
    map["SSID"] = ui.networkName->text();
    map["Passphrase"] = ui.password->text();
    return map;
}

void HiddenNetworkDialog::showPassword(bool checked)
{
    if (!checked)
        ui.password->setEchoMode(QLineEdit::Password);
    else
        ui.password->setEchoMode(QLineEdit::Normal);
}

