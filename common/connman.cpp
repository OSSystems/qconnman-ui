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

#include "connman.h"

#include <dbus/connmanmanager.h>
#include <dbus/connmantechnology.h>
#include <dbus/connmanservice.h>

#include "technology.h"

static Connman *self = NULL;

QDBusArgument &operator<<(QDBusArgument &argument, const ConnmanObject &value)
{
    argument.beginStructure();
    argument << value.path << value.properties;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, ConnmanObject &value)
{
    argument.beginStructure();
    argument >> value.path >> value.properties;
    argument.endStructure();
    return argument;
}

void Connman::init()
{
    qDBusRegisterMetaType<ConnmanObject>();
    qDBusRegisterMetaType<ConnmanTuple>();

    QDBusPendingReply<QVariantMap> reply = m_manager->GetProperties();
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher *)), SLOT(processConnmanProperties(QDBusPendingCallWatcher *)));

    QDBusConnection::systemBus().connect("net.connman", "/", "net.connman.Manager", "TechnologyAdded",
                                         this, SLOT(processNewTechnology(const QDBusObjectPath &, const QVariantMap &)));
}

void Connman::registerAgent(const QString &path)
{
    m_manager->RegisterAgent(QDBusObjectPath(path));
}

QStringList Connman::availableTechnologies() const
{
    QStringList technologies;
    ConnmanTuple list = m_manager->GetTechnologies();
    Q_FOREACH (const ConnmanObject &item, list)
        technologies.append(item.path.path());
    return technologies;
}

QStringList Connman::connectedTechnologies() const
{
    QStringList technologies;
    ConnmanTuple list = m_manager->GetTechnologies();
    Q_FOREACH (const ConnmanObject &item, list)
    {
        QVariantMap properties = ConnmanTechnology("net.connman", item.path.path(), QDBusConnection::systemBus()).GetProperties();
        if (properties["Connected"].toBool())
            technologies.append(item.path.path());
    }
    return technologies;
}

QStringList Connman::services() const
{
    QStringList services;
    QList<QDBusObjectPath> list = qdbus_cast<QList<QDBusObjectPath> >(m_properties["Services"]);
    Q_FOREACH (const QDBusObjectPath &obj, list)
        services.append(obj.path());
    return services;
}

QString Connman::ethernetService()
{
    QStringList services = this->services();
    foreach (const QString &service, services)
    {
        if (serviceType(service) != "ethernet")
            continue;

        return service;
    }

    return QString();
}

void Connman::enableTechnology(const QString &type)
{
    ConnmanTechnology("net.connman", type, QDBusConnection::systemBus(), this).SetProperty("Powered", QDBusVariant(qVariantFromValue(true)));
}

void Connman::disableTechnology(const QString &type)
{
    ConnmanTechnology("net.connman", type, QDBusConnection::systemBus(), this).SetProperty("Powered", QDBusVariant(qVariantFromValue(false)));
}

bool Connman::isTechnologyEnabled(const QString &type)
{
    QVariantMap properties = ConnmanTechnology("net.connman", type, QDBusConnection::systemBus()).GetProperties();
    return properties["Powered"].toBool();
}

bool Connman::isTechnologyConnected(const QString &type)
{
    QVariantMap properties = ConnmanTechnology("net.connman", type, QDBusConnection::systemBus()).GetProperties();
    return properties["Connected"].toBool();
}

QString Connman::technologyPath(const QString &type)
{
    QList<QDBusObjectPath> list = qdbus_cast<QList<QDBusObjectPath> >(m_properties["Technologies"]);
    Q_FOREACH (const QDBusObjectPath &obj, list)
        if (obj.path().endsWith(type))
            return obj.path();
    return QString();
}

void Connman::requestScan(const QString &type)
{
    ConnmanTechnology("net.connman", type, QDBusConnection::systemBus(), this).Scan();
}

QString Connman::serviceType(const QString &service)
{
    ConnmanService c("net.connman", service, QDBusConnection::systemBus(), this);
    QVariantMap properties = c.GetProperties();
    return properties["Type"].toString();
}

Technology *Connman::technology(const QString &path)
{
    if (!m_technologyList.contains(path))
        m_technologyList.insert(path, new Technology(path, this));
    return m_technologyList[path];
}

Connman *Connman::instance()
{
    if (!self)
        self = new Connman;
    return self;
}

Connman::Connman():
    QObject()
{
    m_manager = new ConnmanManager("net.connman", "/", QDBusConnection::systemBus(), this);
}

void Connman::processConnmanProperties(QDBusPendingCallWatcher *watcher)
{
    QDBusPendingReply<QVariantMap> reply = *watcher;
    m_properties = reply.value();

    connect(m_manager, SIGNAL(PropertyChanged(const QString &, const QDBusVariant &)),
            SLOT(propertyChanged(const QString &, const QDBusVariant &)));
}

void Connman::propertyChanged(const QString &name, const QDBusVariant &value)
{
    m_properties[name] = value.variant();

    if (name == "AvailableTechnologies")
        emit technologiesChanged();
    if (name == "EnabledTechnologies")
    {
        emit enabledTechnologiesChanged();
    }
    else if (name == "ConnectedTechnologies")
    {
        emit connectedTechnologiesChanged();
    }
    if (name == "Services")
    {
        QList<QDBusObjectPath> new_services = qdbus_cast<QList<QDBusObjectPath> >(m_properties["Services"]);
        emit servicesChanged();
    }

    qDebug() << "Manager PropertyChanged" << name;
}

void Connman::processNewTechnology(const QDBusObjectPath &obj, const QVariantMap &map)
{
    emit technologiesChanged();
}
