#ifndef _IPV4WIDGET_H
#define _IPV4WIDGET_H

#include "ui_ipv4widget.h"

class Ipv4Widget: public QFrame
{
    Q_OBJECT

public:
    Ipv4Widget(QWidget *parent);

    void setSettings(const QVariantMap &settings);

    QVariantMap toMap();

private slots:
    void on_autoIpAddress_stateChanged(int state);
    void on_manualIpAddress_toggled(bool on);
    void on_autoDns_stateChanged(int state);
    void on_manualDns_toggled(bool on);

private:
    Ui::Ipv4Widget ui;
};

#endif
