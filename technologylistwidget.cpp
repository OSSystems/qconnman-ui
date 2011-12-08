#include "technologylistwidget.h"
#include "technologyitemwidget.h"
#include "connman.h"

#include <QDebug>

TechnologyListWidget::TechnologyListWidget(QWidget *parent):
    QListWidget(parent)
{
    connect(this, SIGNAL(currentRowChanged(int)), SLOT(emitItemChanged(int)));
    connect(Connman::instance(), SIGNAL(technologiesChanged()), SLOT(updateItems()));
    Connman::instance()->init();
}

void TechnologyListWidget::updateItems()
{
    QStringList technologies = Connman::instance()->availableTechnologies();
    foreach (const QString &technology, technologies)
    {
        bool skip = false;

        for (int i = 0; i < count(); ++i)
        {
            QListWidgetItem *item = this->item(i);
            if (item->data(Qt::UserRole).toString() == technology)
                skip = true;
        }

        if (skip)
            continue;

        new TechnologyItemWidget(technology, this);
    }

    QList<int> toRemove;
    for (int i = 0; i < count(); ++i)
    {
        QListWidgetItem *item = this->item(i);
        if (!technologies.contains(item->data(Qt::UserRole).toString()))
            toRemove.append(i);
    }

    foreach (int i, toRemove)
        takeItem(i);
}

void TechnologyListWidget::emitItemChanged(int row)
{
    QListWidgetItem *item = this->item(row);
    emit itemChanged(item->data(Qt::UserRole).toString());
}
