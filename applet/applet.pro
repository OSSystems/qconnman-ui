TARGET = qconnman-applet

include($$PWD/../common/common.pri)

FORMS += $$PWD/wirelessnetworkwidget.ui

SOURCES += $$PWD/main.cpp \
  $$PWD/wirelessnetworkwidget.cpp

HEADERS += $$PWD/wirelessnetworkwidget.h
