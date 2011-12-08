#ifndef _CONNMAN_H
#define _CONNMAN_H

#include <QObject>
#include <QVariantMap>
#include <QDBusVariant>

class QDBusPendingCallWatcher;
class ConnmanManager;

class Connman: public QObject
{
    Q_OBJECT

public:
    void init();

    void registerAgent(const QString &path);

    QStringList availableTechnologies() const;
    QStringList connectedTechnologies() const;
    QStringList services() const;

    QString ethernetService();

    void enableTechnology(const QString &type);
    void disableTechnology(const QString &type);
    bool isTechnologyEnabled(const QString &type);
    bool isTechnologyConnected(const QString &type);
    QString technologyPath(const QString &type);

    void requestScan(const QString &type);

    QString serviceType(const QString &service);

    void connectService(const QVariantMap &map);

    static Connman *instance();

signals:
    void technologiesChanged();
    void enabledTechnologiesChanged();
    void connectedTechnologiesChanged();
    void servicesChanged();

private:
    Connman();

private slots:
    void processConnmanProperties(QDBusPendingCallWatcher *watcher);
    void propertyChanged(const QString &name, const QDBusVariant &value);

private:
    ConnmanManager *m_manager;
    QVariantMap m_properties;
};

#endif
