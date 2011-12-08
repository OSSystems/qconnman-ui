#include "networkitemwidget.h"
#include "service.h"

#include <QListWidget>
#include <QDebug>

NetworkItemWidget::NetworkItemWidget(const QString &service, QListWidget *parent):
    QWidget(parent)
{
    ui.setupUi(this);

    m_item = new QListWidgetItem(parent);
    parent->setItemWidget(m_item, this);
    m_item->setSizeHint(sizeHint());
    m_item->setData(Qt::UserRole, service);

    m_service = new Service(service, this);

    update();

    connect(m_service, SIGNAL(propertiesChanged()), SLOT(update()));
}

void NetworkItemWidget::update()
{
    if (!m_service->isHidden())
        ui.ssid->setText(m_service->name());
    else
        ui.ssid->setText("Hidden wireless network");
    ui.secured->setVisible(!m_service->security().contains("none"));
    ui.unsecured->setVisible(m_service->security().contains("none"));
    ui.securityIcon->setVisible(!m_service->security().contains("none"));
    ui.secured->setText(QString("Security-enabled network (%1)").arg(m_service->security().join(" ").toUpper()));

    ui.strength->setValue(m_service->strength());
}

