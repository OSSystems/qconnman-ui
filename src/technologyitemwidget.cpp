#include "technologyitemwidget.h"

#include <QListWidget>
#include <QDebug>

TechnologyItemWidget::TechnologyItemWidget(const QString &technology, QListWidget *parent):
    QWidget(parent)
{
    ui.setupUi(this);

    if (technology == "ethernet")
    {
        ui.icon->setPixmap(QPixmap(":/images/network-wired.png"));
        ui.label->setText("Wired");
    }
    else if (technology == "wifi")
    {
        ui.icon->setPixmap(QPixmap(":/images/network-wireless.png"));
        ui.label->setText("Wireless");
    }

    m_item = new QListWidgetItem(parent);
    parent->setItemWidget(m_item, this);
    m_item->setSizeHint(sizeHint());
    m_item->setData(Qt::UserRole, technology);
}
