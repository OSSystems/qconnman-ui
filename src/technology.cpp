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

