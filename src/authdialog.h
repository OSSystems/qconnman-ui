#ifndef _AUTHDIALOG_H
#define _AUTHDIALOG_H

#include "ui_authdialog.h"

class ConnMan;

class AuthDialog: public QDialog
{
    Q_OBJECT

public:
    AuthDialog(ConnMan *manager, QWidget *parent);

public slots:
    int exec();

private slots:
    void showPassword(bool checked);

private:
    Ui::AuthDialog ui;
    ConnMan *m_manager;
};

#endif
