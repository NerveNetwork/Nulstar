# Base Settings
include($$TOP_SRCDIR/common.pri)

# Project Settings
QT -= gui
QT += websockets
CONFIG += c++11 console

# Target Settings

TARGET = ServiceManager
VERSION = 0.1.0
APP_DOMAIN=Nulstar
APP_ROLES=Role_ServiceManager[0.1]
APP_PROTOCOL_VERSIONS=0.1
APP_NAME=$$TARGET
APP_VERSION=$$VERSION
APP_VERSION_NAME=Baby Apple I


DESTDIR=$$MODULES_OUTDIR/$$APP_DOMAIN/$$TARGET/$$VERSION

# Install Settings

OTHER_FILES += Module.ncf
COPYFILE1 = \"$$PWD/Module.ncf\" \"$$DESTDIR\"
win32: COPYFILE1 ~= s,/,\\,g
QMAKE_POST_LINK += $$QMAKE_COPY $$COPYFILE1

# Source Files

QMAKE_SUBSTITUTES += ServiceManagerVersion.h.in

HEADERS += NServiceManagerController.h

SOURCES += NServiceManagerController.cpp \
           ServiceManager.cpp

# Dependencies

INCLUDEPATH += $$LIBRARIES_PATH/Nulstar/NNetwork \
               $$LIBRARIES_PATH/Nulstar/NCore  \
               $$LIBRARIES_PATH/Nulstar/NTools
LIBS += -lNNetwork -lNCore -lNTools
