#ifndef _SERVICE_H
#define _SERVICE_H

#include <QObject>
#include <QDBusVariant>

class QDBusPendingCallWatcher;
class ConnmanService;

class Service: public QObject
{
    Q_OBJECT

public:
    Service(const QString &path, QObject *parent);

    QString path() const;
    QString name() const;
    bool isHidden() const;
    QString state() const;
    QString type() const;
    QStringList security() const;
    QString passphrase() const;
    void setPassphrase(const QString &passphrase);
    quint8 strength() const;

    void setAutoConnect(bool value);

    QVariantMap ipv4Settings();
    void setIpv4Settings(const QVariantMap &map);

    void connect();
    void disconnect();

signals:
    void propertiesChanged();

private slots:
    void processProperties(QDBusPendingCallWatcher *watcher);
    void propertyChanged(const QString &name, const QDBusVariant &value);

private:
    ConnmanService *m_service;
    QVariantMap m_properties;
};

#endif
