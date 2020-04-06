QT += quick sql

CONFIG += c++17 console -Wno-deprecated-declarations

LIBS += -L/usr/lib -lssl -lcrypto -I/usr/include

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    config.cpp \
    core/database.cpp \
    core/handler.cpp \
    core/model.cpp \
    core/securesocket.cpp \
    core/torrentfileparser.cpp \
    handlers/adminHandler.cpp \
    handlers/hitandrunHandler.cpp \
    handlers/inviteHandler.cpp \
    handlers/loginHandler.cpp \
    handlers/textHandler.cpp \
    handlers/torrentHandler.cpp \
    handlers/uploadhandler.cpp \
    main.cpp \
    model/user.cpp \

RESOURCES += qml.qrc \

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = \
        PushButton.qml
        header.qml

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH = \
        PushButton.qml
        header.qml

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    core/bencode.hpp \
    core/database.h \
    core/handler.h \
    core/model.h \
    core/securesocket.h \
    core/torrentfileparser.h \
    handlers/adminHandler.h \
    handlers/hitandrunHandler.h \
    handlers/inviteHandler.h \
    handlers/loginHandler.h \
    handlers/textHandler.h \
    handlers/torrentHandler.h \
    handlers/uploadhandler.h \
    model/user.h \

DISTFILES += \
    makeTables.sql
