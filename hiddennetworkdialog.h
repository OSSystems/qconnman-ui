#ifndef _HIDDENNETWORKDIALOG_H
#define _HIDDENNETWORKDIALOG_H

#include "ui_hiddennetworkdialog.h"

class HiddenNetworkDialog: public QDialog
{
    Q_OBJECT

public:
    HiddenNetworkDialog(const QString &security, QWidget *parent);

    QVariantMap toMap();

private slots:
    void showPassword(bool checked);

private:
    Ui::HiddenNetworkDialog ui;
};

#endif
