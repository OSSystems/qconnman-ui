#include "networklistwidget.h"
#include "networkitemwidget.h"
#include "connman.h"
#include "service.h"

#include <QTimer>
#include <QDebug>

NetworkListWidget::NetworkListWidget(QWidget *parent):
    QListWidget(parent)
{
    updateItems();
    connect(Connman::instance(), SIGNAL(servicesChanged()), SLOT(updateItems()));

    QTimer *timer = new QTimer(this);
    timer->start(30000);
    connect(timer, SIGNAL(timeout()), SLOT(scan()));
    scan();
}

QString NetworkListWidget::selectedNetwork() const
{
    QListWidgetItem *item = currentItem();
    if (!item)
        return QString();
    return item->data(Qt::UserRole).toString();
}

void NetworkListWidget::updateItems()
{
    QStringList services = Connman::instance()->services();
    foreach (const QString &service, services)
    {
        if (Connman::instance()->serviceType(service) != "wifi")
            continue;

        bool skip = false;

        for (int i = 0; i < count(); ++i)
        {
            QListWidgetItem *item = this->item(i);
            if (item->data(Qt::UserRole).toString() == service)
                skip = true;
        }

        if (skip)
            continue;

        new NetworkItemWidget(service, this);
    }

    QList<int> toRemove;
    for (int i = 0; i < count(); ++i)
    {
        QListWidgetItem *item = this->item(i);
        if (!services.contains(item->data(Qt::UserRole).toString()))
            toRemove.append(i);
    }

    foreach (int i, toRemove)
        takeItem(i);
}

void NetworkListWidget::scan()
{
    qDebug("RequestScan");
    Connman::instance()->requestScan("wifi");
}

