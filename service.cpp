#include "service.h"

#include <connman/connmanservice.h>

#include <QDebug>

typedef QMap<QString, QString> StringMap;
Q_DECLARE_METATYPE(StringMap);

Service::Service(const QString &path, QObject *parent):
    QObject(parent)
{
    m_service = new ConnmanService("net.connman", path, QDBusConnection::systemBus(), this);

    qDBusRegisterMetaType<StringMap>();

    QDBusReply<QVariantMap> reply = m_service->GetProperties();
    m_properties = reply.value();

    QObject::connect(m_service, SIGNAL(PropertyChanged(const QString &, const QDBusVariant &)),
                     SLOT(propertyChanged(const QString &, const QDBusVariant &)));

}

QString Service::path() const
{
    return m_service->path();
}

QString Service::name() const
{
    return m_properties["Name"].toString();
}

bool Service::isHidden() const
{
    return name().isEmpty();
}

QString Service::state() const
{
    return m_properties["State"].toString();
}

QString Service::type() const
{
    return m_properties["Type"].toString();
}

QStringList Service::security() const
{
    return m_properties["Security"].toStringList();
}

QString Service::passphrase() const
{
    return m_properties["Passphrase"].toString();
}

void Service::setPassphrase(const QString &passphrase)
{
    QDBusPendingReply<void> reply = m_service->SetProperty("Passphrase", QDBusVariant(passphrase));
    reply.waitForFinished();
}

quint8 Service::strength() const
{
    return m_properties["Strength"].value<quint8>();
}

void Service::setAutoConnect(bool value)
{
    m_service->SetProperty("AutoConnect", QDBusVariant(qVariantFromValue(value)));
}

QVariantMap Service::ipv4Settings()
{
    QVariantMap properties = m_service->GetProperties();
    return qdbus_cast<QVariantMap>(properties["IPv4"]);
}

void Service::setIpv4Settings(const QVariantMap &map)
{
    StringMap settings;
    Q_FOREACH (const QString &key, map.keys())
        settings.insert(key, map.value(key).toString());

    m_service->SetProperty("IPv4.Configuration", QDBusVariant(qVariantFromValue(settings)));
}

void Service::connect()
{
    m_service->Connect();
}

void Service::disconnect()
{
    m_service->Disconnect();
}

void Service::processProperties(QDBusPendingCallWatcher *watcher)
{
}

void Service::propertyChanged(const QString &name, const QDBusVariant &value)
{
    qDebug() << "Service PropertyChanged" << name;

    m_properties[name] = value.variant();

    emit propertiesChanged();
}

