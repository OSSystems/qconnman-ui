QT += dbus

TARGET = qconnman-ui

FORMS += $$PWD/mainwindow.ui \
  $$PWD/wirelesspage.ui \
  $$PWD/wiredpage.ui \
  $$PWD/ipv4widget.ui \
  $$PWD/ipv4configurationdialog.ui \
  $$PWD/authdialog.ui

SOURCES += $$PWD/main.cpp \
  $$PWD/mainwindow.cpp \
  $$PWD/wirelesspage.cpp \
  $$PWD/wiredpage.cpp \
  $$PWD/ipv4widget.cpp \
  $$PWD/spinnerwidget.cpp \
  $$PWD/widgetfilter.cpp \
  $$PWD/ipv4configurationdialog.cpp \
  $$PWD/authdialog.cpp

HEADERS += $$PWD/mainwindow.h \
  $$PWD/wirelesspage.h \
  $$PWD/wiredpage.h \
  $$PWD/ipv4widget.h \
  $$PWD/spinnerwidget.h \
  $$PWD/widgetfilter.h \
  $$PWD/ipv4configurationdialog.h \
  $$PWD/authdialog.h

LIBS += -lqconnman
