TARGET = qconnman-ui

include($$PWD/../common/common.pri)

FORMS += $$PWD/mainwindow.ui \
  $$PWD/technologyitemwidget.ui \
  $$PWD/wirelesspage.ui \
  $$PWD/wiredpage.ui \
  $$PWD/networkitemwidget.ui \
  $$PWD/ipv4widget.ui

SOURCES += $$PWD/main.cpp \
  $$PWD/mainwindow.cpp \
  $$PWD/technologylistwidget.cpp \
  $$PWD/technologyitemwidget.cpp \
  $$PWD/wirelesspage.cpp \
  $$PWD/wiredpage.cpp \
  $$PWD/networkitemwidget.cpp \
  $$PWD/networklistwidget.cpp \
  $$PWD/ipv4widget.cpp \
  $$PWD/spinnerwidget.cpp

HEADERS += $$PWD/mainwindow.h \
  $$PWD/technologylistwidget.h \
  $$PWD/technologyitemwidget.h \
  $$PWD/wirelesspage.h \
  $$PWD/wiredpage.h \
  $$PWD/networkitemwidget.h \
  $$PWD/networklistwidget.h \
  $$PWD/ipv4widget.h \
  $$PWD/spinnerwidget.h

LIBS += -lqconnman
