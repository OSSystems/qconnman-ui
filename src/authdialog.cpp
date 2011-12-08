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

#include "authdialog.h"

#include <QDebug>

AuthDialog::AuthDialog(const QString &security, const QString &password, QWidget *parent):
    QDialog(parent)
{
    ui.setupUi(this);
    ui.label->setText(ui.label->text().arg(security));
    ui.password->setText(password);

    connect(ui.showPassword, SIGNAL(toggled(bool)), SLOT(showPassword(bool)));
}

QString AuthDialog::password()
{
    return ui.password->text();
}

void AuthDialog::showPassword(bool checked)
{
    if (!checked)
        ui.password->setEchoMode(QLineEdit::Password);
    else
        ui.password->setEchoMode(QLineEdit::Normal);
}

