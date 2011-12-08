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

#include "technology.h"

#include <dbus/connmantechnology.h>

Technology::Technology(const QString &path, QObject *parent):
    QObject(parent)
{
    m_technology = new ConnmanTechnology("net.connman", path, QDBusConnection::systemBus(), this);

    QDBusPendingReply<QVariantMap> reply = m_technology->GetProperties();
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
    QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher *)), SLOT(processProperties(QDBusPendingCallWatcher *)));
}

QString Technology::state()
{
    return m_properties["State"].toString();
}

void Technology::processProperties(QDBusPendingCallWatcher *watcher)
{
    QDBusPendingReply<QVariantMap> reply = *watcher;
    m_properties = reply.value();

    QObject::connect(m_technology, SIGNAL(PropertyChanged(const QString &, const QDBusVariant &)),
                     SLOT(propertyChanged(const QString &, const QDBusVariant &)));

    emit propertiesChanged();
}

void Technology::propertyChanged(const QString &name, const QDBusVariant &value)
{
    m_properties[name] = value.variant();

    if (name == "State")
        emit stateChanged();

    emit propertiesChanged();
}

