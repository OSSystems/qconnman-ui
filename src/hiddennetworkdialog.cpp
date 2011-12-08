#include "hiddennetworkdialog.h"

HiddenNetworkDialog::HiddenNetworkDialog(const QString &security, QWidget *parent):
    QDialog(parent)
{
    ui.setupUi(this);

    ui.label->setText(ui.label->text().arg(security));

    connect(ui.showPassword, SIGNAL(toggled(bool)), SLOT(showPassword(bool)));
}

QVariantMap HiddenNetworkDialog::toMap()
{
    QVariantMap map;
    map["Name"] = ui.networkName->text();
    map["SSID"] = ui.networkName->text();
    map["Passphrase"] = ui.password->text();
    return map;
}

void HiddenNetworkDialog::showPassword(bool checked)
{
    if (!checked)
        ui.password->setEchoMode(QLineEdit::Password);
    else
        ui.password->setEchoMode(QLineEdit::Normal);
}

