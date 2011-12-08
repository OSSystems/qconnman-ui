#include "authdialog.h"

#include <QDebug>

AuthDialog::AuthDialog(const QString &security, const QString &password, QWidget *parent):
    QDialog(parent)
{
    ui.setupUi(this);
    ui.label->setText(ui.label->text().arg(security));
    ui.password->setText(password);

    connect(ui.showPassword, SIGNAL(toggled(bool)), SLOT(showPassword(bool)));
}

QString AuthDialog::password()
{
    return ui.password->text();
}

void AuthDialog::showPassword(bool checked)
{
    if (!checked)
        ui.password->setEchoMode(QLineEdit::Password);
    else
        ui.password->setEchoMode(QLineEdit::Normal);
}

