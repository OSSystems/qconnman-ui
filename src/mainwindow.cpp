#include "mainwindow.h"
#include "wiredpage.h"
#include "wirelesspage.h"
#include "agent.h"
#include "connman.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent):
    QWidget(parent)
{
    ui.setupUi(this);

    new Agent(this);
    Connman::instance()->registerAgent("/");

    connect(ui.technologyListWidget, SIGNAL(itemChanged(const QString &)), SLOT(changePage(const QString &)));
}

void MainWindow::changePage(const QString &technology)
{
    QWidget *widget = m_pages.value(technology);
    if (!widget)
    {
        if (technology == "ethernet")
        {
            widget = new WiredPage(this);
            ui.stackedWidget->addWidget(widget);
            m_pages.insert(technology, widget);
        }
        else if (technology == "wifi")
        {
            widget = new WirelessPage(this);
            ui.stackedWidget->addWidget(widget);
            m_pages.insert(technology, widget);
        }
    }

    ui.stackedWidget->setCurrentWidget(widget);
}

