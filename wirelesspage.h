#ifndef _WIRELESSPAGE_H
#define _WIRELESSPAGE_

#include "ui_wirelesspage.h"

class Technology;

class WirelessPage: public QWidget
{
    Q_OBJECT

public:
    WirelessPage(QWidget *parent);

private:
    QString connectedServicePath();

private slots:
    void updateState();

    void networkListItemChanged(int row);

    void toggleTechnology(bool checked);

    void connectToNetwork();
    void disconnect();

    void showAddNetworkDialog();

private:
    Ui::WirelessPage ui;
    Technology *m_technology;
};

#endif
