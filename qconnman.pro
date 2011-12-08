QT += dbus
TEMPLATE = app

RESOURCES += qconnman.qrc

FORMS += mainwindow.ui technologyitemwidget.ui wirelesspage.ui wiredpage.ui networkitemwidget.ui authdialog.ui hiddennetworkdialog.ui ipv4widget.ui
SOURCES += connman/connmanmanager.cpp connman.cpp mainwindow.cpp technologylistwidget.cpp main.cpp technologyitemwidget.cpp wirelesspage.cpp wiredpage.cpp networkitemwidget.cpp networklistwidget.cpp connman/connmanservice.cpp service.cpp connman/connmantechnology.cpp technology.cpp authdialog.cpp hiddennetworkdialog.cpp ipv4widget.cpp connman/connmanagent.cpp agent.cpp

HEADERS += connman/connmanmanager.h connman.h mainwindow.h technologylistwidget.h technologyitemwidget.h wirelesspage.h wiredpage.h networkitemwidget.h networklistwidget.h connman/connmanservice.h service.h connman/connmantechnology.h technology.h authdialog.h hiddennetworkdialog.h ipv4widget.h connman/connmanagent.h agent.h
