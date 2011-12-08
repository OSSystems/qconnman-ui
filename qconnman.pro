QT += dbus
TEMPLATE = app

UI_DIR = .ui
MOC_DIR = .moc
OBJECTS_DIR = .obj

INCLUDEPATH += $$PWD/.ui $$PWD/src/

RESOURCES += qconnman.qrc

include(ui/ui.pri)
include(src/dbus/dbus.pri)
include(src/src.pri)
