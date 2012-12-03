#ifndef _AUTHDIALOG_H
#define _AUTHDIALOG_H

#include "ui_authdialog.h"

class Manager;

class AuthDialog: public QDialog
{
    Q_OBJECT

public:
    AuthDialog(Manager *manager, QWidget *parent);

public slots:
    int exec();

private slots:
    void showPassword(bool checked);

private:
    Ui::AuthDialog ui;
    Manager *m_manager;
};

#endif
