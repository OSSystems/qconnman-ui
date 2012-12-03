#include "authdialog.h"

#include <qconnman/manager.h>
#include <qconnman/agent.h>

#include <QDebug>

AuthDialog::AuthDialog(Manager *manager, QWidget *parent):
    QDialog(parent),
    m_manager(manager)
{
    ui.setupUi(this);

    connect(ui.showPassword, SIGNAL(toggled(bool)), SLOT(showPassword(bool)));
}

int AuthDialog::exec()
{
    Agent *agent = qobject_cast<Agent *>(QObject::sender());
    Agent::InputRequest *request = agent->currentInputRequest();

    ui.password->clear();
    ui.password->setFocus();
    ui.label->setText(trUtf8("This network requires a %1 password to connect. Please enter the password bellow.")
                      .arg(m_manager->service(request->service)->security().join("").toUpper()));

    int result = QDialog::exec();
    if (result == QDialog::Accepted)
        request->response.passphrase = ui.password->text();
    else
        request->cancel = true;

    return result;
}

void AuthDialog::showPassword(bool checked)
{
    if (!checked)
        ui.password->setEchoMode(QLineEdit::Password);
    else
        ui.password->setEchoMode(QLineEdit::Normal);
}
