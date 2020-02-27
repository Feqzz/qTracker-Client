QT += core gui
QT  += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 console

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    app/controller/logincontroller.cpp \
    config.cpp \
    app/core/application.cpp \
    app/core/controller.cpp \
    app/core/database.cpp \
    app/core/model.cpp \
    app/core/view.cpp \
    app/model/login.cpp \
    app/model/user.cpp \
    app/view/loginwindow.cpp \
    public/main.cpp

HEADERS += \
    app/controller/logincontroller.h \
    app/core/application.h \
    app/core/controller.h \
    app/core/database.h \
    app/core/model.h \
    app/core/view.h \
    app/model/login.h \
    app/model/user.h \
    app/view/loginwindow.h

FORMS += \
    loginwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    makeTables.sql
