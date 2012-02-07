TARGET = qconnman-applet

QT += network

include($$PWD/../common/common.pri)

FORMS += $$PWD/wirelessnetworkwidget.ui

SOURCES += $$PWD/../common/main.cpp \
  $$PWD/trayicon.cpp \
  $$PWD/wirelessnetworkwidget.cpp

HEADERS += $$PWD/trayicon.h \
  $$PWD/wirelessnetworkwidget.h
