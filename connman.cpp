#include "connman.h"

#include <connman/connmanmanager.h>
#include <connman/connmanservice.h>

static Connman *self = NULL;

void Connman::init()
{
    QDBusPendingReply<QVariantMap> reply = m_manager->GetProperties();
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher *)), SLOT(processConnmanProperties(QDBusPendingCallWatcher *)));
}

void Connman::registerAgent(const QString &path)
{
    m_manager->RegisterAgent(QDBusObjectPath(path));
}

QStringList Connman::availableTechnologies() const
{
    return m_properties["AvailableTechnologies"].toStringList();
}

QStringList Connman::connectedTechnologies() const
{
    return m_properties["ConnectedTechnologies"].toStringList();
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
    m_manager->EnableTechnology(type);
}

void Connman::disableTechnology(const QString &type)
{
    m_manager->DisableTechnology(type);
}

bool Connman::isTechnologyEnabled(const QString &type)
{
    return m_properties["EnabledTechnologies"].toStringList().contains(type);
}

bool Connman::isTechnologyConnected(const QString &type)
{
    return m_properties["ConnectedTechnologies"].toStringList().contains(type);
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
    m_manager->RequestScan(type);
}

QString Connman::serviceType(const QString &service)
{
    ConnmanService c("net.connman", service, QDBusConnection::systemBus(), this);
    QVariantMap properties = c.GetProperties();
    return properties["Type"].toString();
}

void Connman::connectService(const QVariantMap &map)
{
    QDBusPendingReply<QDBusObjectPath> reply = m_manager->ConnectService(map);
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

//    QList<QDBusObjectPath> new_services = qdbus_cast<QList<QDBusObjectPath> >(m_properties["Services"]);
//    qDebug() << new_services.size();


//    qDebug() << m_properties["Services"].;

    emit technologiesChanged();
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


