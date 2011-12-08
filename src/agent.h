#ifndef _AGENT_H
#define _AGENT_H

#include <QObject>
#include <QDBusObjectPath>
#include <QVariantMap>
#include <QDBusContext>

class Agent: public QObject, protected QDBusContext
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "net.connman.Agent")

public:
    Agent(QObject *parent);

public slots:
    void Release();
    QVariantMap RequestInput(const QDBusObjectPath &servicePath, const QVariantMap &fields);
    void ReportError(const QDBusObjectPath &servicePath, const QString &error);
};

#endif
