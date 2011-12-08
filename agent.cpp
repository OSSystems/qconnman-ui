#include "agent.h"
#include "hiddennetworkdialog.h"
#include "authdialog.h"
#include "service.h"

#include <connman/connmanagent.h>

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
}

