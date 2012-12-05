QT += dbus

TARGET = qconnman-ui

target.path = $$PREFIX/bin

UI_DIR = .ui
MOC_DIR = .moc
OBJECTS_DIR = .obj
RCC_DIR = .rcc

RESOURCES += $$PWD/../qconnman-ui.qrc
TRANSLATIONS = $$PWD/../i18n/pt_BR.ts

translations.name = Translations ${QMAKE_FILE_BASE}
translations.CONFIG += no_link target_predeps
translations.commands = @$$[QT_INSTALL_BINS]/lrelease ${QMAKE_FILE_IN} -qm .moc/${QMAKE_FILE_OUT} &> /dev/null
translations.input = TRANSLATIONS
translations.output = ${QMAKE_FILE_BASE}.qm
translations.clean = ${QMAKE_FILE_BASE}.qm
QMAKE_EXTRA_COMPILERS += translations

FORMS += $$PWD/mainwindow.ui \
  $$PWD/wirelesspage.ui \
  $$PWD/wiredpage.ui \
  $$PWD/ipv4widget.ui \
  $$PWD/ipv4configurationdialog.ui \
  $$PWD/authdialog.ui \
  $$PWD/hiddennetworkinputdialog.ui

SOURCES += $$PWD/main.cpp \
  $$PWD/connman.cpp \
  $$PWD/mainwindow.cpp \
  $$PWD/wirelesspage.cpp \
  $$PWD/wiredpage.cpp \
  $$PWD/ipv4widget.cpp \
  $$PWD/spinnerwidget.cpp \
  $$PWD/widgetfilter.cpp \
  $$PWD/ipv4configurationdialog.cpp \
  $$PWD/authdialog.cpp \
  $$PWD/hiddennetworkinputdialog.cpp

HEADERS += $$PWD/connman.h \
  $$PWD/mainwindow.h \
  $$PWD/wirelesspage.h \
  $$PWD/wiredpage.h \
  $$PWD/ipv4widget.h \
  $$PWD/spinnerwidget.h \
  $$PWD/widgetfilter.h \
  $$PWD/ipv4configurationdialog.h \
  $$PWD/authdialog.h \
  $$PWD/hiddennetworkinputdialog.h

LIBS += -lqconnman

qm.files = $$PWD/.moc/*.qm
qm.path = $$PREFIX/share/qconnman/i18n
qm.CONFIG += no_check_exist

INSTALLS += target qm
