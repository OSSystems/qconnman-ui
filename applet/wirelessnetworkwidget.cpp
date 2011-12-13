#include "wirelessnetworkwidget.h"

WirelessNetworkWidget::WirelessNetworkWidget(const QString &servicePath, QWidget *parent):
    QWidget(parent)
{
    ui.setupUi(this);

    setMouseTracking(true);
//    setBackgroundRole(QPalette::Base);
    setForegroundRole(QPalette::BrightText);
}

bool WirelessNetworkWidget::event(QEvent *event)
{
   switch (event->type())
   {
       case QEvent::Enter:
           setForegroundRole(QPalette::BrightText);
           setBackgroundRole(QPalette::Highlight);
           update();
           emit hovered();
           break;
       case QEvent::Leave:
           setBackgroundRole(QPalette::Window);
           update();
           emit leave();
           break;
       default:
           return QWidget::event(event);
   }

   return true;
}

