#include "connman.h"

#include <QIcon>

ConnMan::ConnMan(QObject *parent):
    Manager(false, parent)
{
    initialize();
}

QIcon ConnMan::serviceIcon(Service *service) const
{
    if (service->type().toLower() == "wifi")
    {
        quint8 strength = service->strength();
        QString secure = service->security().contains("none") ? "" : "secure";
        if (strength > 80)
            return QIcon::fromTheme(QString("nm-signal-100-%1").arg(secure));
        else if (strength > 55)
            return QIcon::fromTheme(QString("nm-signal-75-%1").arg(secure));
        else if (strength > 30)
            return QIcon::fromTheme(QString("nm-signal-50-%1").arg(secure));
        else if (strength > 5)
            return QIcon::fromTheme(QString("nm-signal-25-%1").arg(secure));
        else
            return QIcon::fromTheme(QString("nm-signal-00-%1").arg(secure));
    }

    return Manager::serviceIcon(service);
}
