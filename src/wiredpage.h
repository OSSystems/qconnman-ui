#ifndef _WIREDPAGE_H
#define _WIREDPAGE_H

#include "ui_wiredpage.h"

class WiredPage: public QWidget
{
    Q_OBJECT

public:
    WiredPage(QWidget *parent);

private slots:
    void updateButtonsVisibility();

    void toggleTechnology(bool checked);

    void connect();
    void disconnect();

private:
    Ui::WiredPage ui;
};

#endif
