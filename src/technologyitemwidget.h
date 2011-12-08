#ifndef _TECHNOLOGYITEMWIDGET_H
#define _TECHNOLOGYITEMWIDGET_H

#include "ui_technologyitemwidget.h"

class QListWidget;
class QListWidgetItem;

class TechnologyItemWidget: public QWidget
{
public:
    TechnologyItemWidget(const QString &technology, QListWidget *parent);

private:
    Ui::TechnologyItemWidget ui;
    QListWidgetItem *m_item;
};

#endif
