#ifndef _AGENT_H
#define _AGENT_H

#include <QObject>
#include <QDBusObjectPath>
#include <QVariantMap>

class Agent: public QObject
{
    Q_OBJECT

public:
    Agent(QObject *parent);

public slots:
    void Release();
    QVariantMap RequestInput(const QDBusObjectPath &servicePath, const QVariantMap &fields);
    void ReportError(const QDBusObjectPath &servicePath, const QString &error);
};

#endif
