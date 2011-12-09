QT += dbus
TEMPLATE = app

TARGET = qconnman

UI_DIR = .ui
MOC_DIR = .moc
OBJECTS_DIR = .obj
RCC_DIR = .rcc

INCLUDEPATH += $$PWD/.ui $$PWD/src/

RESOURCES += qconnman.qrc
TRANSLATIONS = $$PWD/i18n/pt_BR.ts

translations.name = Translations ${QMAKE_FILE_BASE}
translations.CONFIG += no_link target_predeps
translations.commands = @$$[QT_INSTALL_BINS]/lrelease ${QMAKE_FILE_IN} -qm .moc/${QMAKE_FILE_OUT} &> /dev/null
translations.input = TRANSLATIONS
translations.output = ${QMAKE_FILE_BASE}.qm
translations.clean = ${QMAKE_FILE_BASE}.qm
QMAKE_EXTRA_COMPILERS += translations

include(ui/ui.pri)
include(src/dbus/dbus.pri)
include(src/src.pri)

target.path = $$PREFIX/bin

qm.files = $$PWD/.moc/*.qm
qm.path = $$PREFIX/share/qconnman/i18n
qm.CONFIG += no_check_exist

INSTALLS += target qm
