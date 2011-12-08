#ifndef _NETWORKITEMWIDGET_H
#define _NETWORKITEMWIDGET_H

#include "ui_networkitemwidget.h"

class QListWidget;
class QListWidgetItem;
class Service;

class NetworkItemWidget: public QWidget
{
    Q_OBJECT

public:
    NetworkItemWidget(const QString &service, QListWidget *parent);

private slots:
    void update();

private:
    Ui::NetworkItemWidget ui;
    QListWidgetItem *m_item;
    Service *m_service;
};
#endif
