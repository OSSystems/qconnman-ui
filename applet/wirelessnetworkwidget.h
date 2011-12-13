#ifndef _WIRELESSNETWORKWIDGET_H
#define _WIRELESSNETWORKWIDGET_H

#include "ui_wirelessnetworkwidget.h"

class WirelessNetworkWidget: public QWidget
{
    Q_OBJECT

public:
    WirelessNetworkWidget(const QString &servicePath, QWidget *parent = 0);

signals:
    void hovered();
    void leave();

protected:
    virtual bool event(QEvent *event);

private:
    Ui::WirelessNetworkWidget ui;
};

#endif
