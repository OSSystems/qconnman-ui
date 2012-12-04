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

    Service *service = m_manager->service(request->service);

    ui.password->clear();
    ui.password->setFocus();

    if (!service->name().isEmpty())
        ui.label->setText(trUtf8("The '%1' network requires an access password.").arg(service->name()));
    else
        ui.label->setText(trUtf8("The '%1' network requires an access password.").arg(request->response.name));

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
