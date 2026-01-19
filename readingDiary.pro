QT       += core gui widgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    database.cpp \
    dialogs/logindialog.cpp \
    dialogs/newaccountdialog.cpp \
    dialogs/newbookdialog.cpp \
    dialogs/newsectiondialog.cpp \
    main.cpp \
    mainwindow.cpp \
    models/book.cpp \
    models/group.cpp \
    models/section.cpp \
    models/settings.cpp \
    models/user.cpp

HEADERS += \
    database.h \
    dialogs/logindialog.h \
    dialogs/newaccountdialog.h \
    dialogs/newbookdialog.h \
    dialogs/newsectiondialog.h \
    mainwindow.h \
    models/book.h \
    models/group.h \
    models/section.h \
    models/settings.h \
    models/user.h

TRANSLATIONS = English.ts Ukranian(Українська).ts

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RESOURCES += \
    resources.qrc
