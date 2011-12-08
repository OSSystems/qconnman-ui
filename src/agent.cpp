/*
 * qconnman - Connman Applet
 * Copyright (C) 2011 O.S. Systems
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "agent.h"
#include "hiddennetworkdialog.h"
#include "authdialog.h"
#include "service.h"

#include <dbus/connmanagent.h>

#include <QMessageBox>
#include <QDebug>

Agent::Agent(QObject *parent):
    QObject(parent), QDBusContext()
{
    new ConnmanAgent(this);

    QDBusConnection connection = QDBusConnection::systemBus();
    connection.registerObject("/", this);
    connection.registerService("net.connman.Agent");
}

void Agent::Release()
{
}

QVariantMap Agent::RequestInput(const QDBusObjectPath &servicePath, const QVariantMap &fields)
{
    QVariantMap map;

    qDebug() << "RequestInput fields" << fields.keys();

    if (fields.contains("SSID"))
    {
        HiddenNetworkDialog dialog(Service(servicePath.path(), this).security().join(" ").toUpper(), qobject_cast<QWidget *>(parent()));
        if (dialog.exec() != QDialog::Accepted)
            return map;

        map.unite(dialog.toMap());
    }
    else if (fields.contains("Passphrase"))
    {
        Service service(servicePath.path(), this);
        AuthDialog dialog(service.security().join(" ").toUpper(), service.passphrase(), qobject_cast<QWidget *>(parent()));
        if (dialog.exec() != QDialog::Accepted)
        {
            sendErrorReply("net.connman.Agent.Error.Cancel", "");
            return fields;
        }

        map["Passphrase"] = dialog.password();
    }

    qDebug() << "RequestInput reply" << map;

    return map;
}

void Agent::ReportError(const QDBusObjectPath &servicePath, const QString &error)
{
    qDebug() << "ReportError" << servicePath.path() << error;

    if (error == "invalid-key")
    {
        Service service(servicePath.path(), this);
        QMessageBox::critical(qobject_cast<QWidget *>(parent()), "Invalid password",
                              QString("Unable to connect to '%1' wireless network because the entered password is invalid.").arg(service.name()));
    }
}

