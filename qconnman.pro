QT += dbus
TEMPLATE = app

TARGET = qconnman

UI_DIR = .ui
MOC_DIR = .moc
OBJECTS_DIR = .obj
RCC_DIR = .rcc

INCLUDEPATH += $$PWD/.ui $$PWD/src/

RESOURCES += qconnman.qrc

include(ui/ui.pri)
include(src/dbus/dbus.pri)
include(src/src.pri)

target.path = $$PREFIX/bin

INSTALLS += target
