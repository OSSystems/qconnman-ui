#ifndef _AUTHDIALOG_H
#define _AUTHDIALOG_H

#include "ui_authdialog.h"

class AuthDialog: public QDialog
{
    Q_OBJECT

public:
    AuthDialog(const QString &security, const QString &password, QWidget *parent);

    QString password();

private slots:
    void showPassword(bool checked);

private:
    Ui::AuthDialog ui;
};

#endif
