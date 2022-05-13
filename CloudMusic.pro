#-------------------------------------------------
#
# Project created by QtCreator 2022-04-26T14:19:31
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CloudMusic
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_ICONS = ooopic_1520340986.ico

SOURCES += \
        main.cpp \
        MusicInterface.cpp \
    HttpHandle.cpp \
    JsonAnalyze.cpp \
    MusicPlayer.cpp \
    GraphicsView.cpp \
    DiskItem.cpp \
    NeedleItem.cpp \
    AboutInterface.cpp

HEADERS += \
        MusicInterface.h \
    HttpHandle.h \
    JsonAnalyze.h \
    MusicPlayer.h \
    GraphicsView.h \
    DiskItem.h \
    NeedleItem.h \
    AboutInterface.h

FORMS += \
        MusicInterface.ui \
    AboutInterface.ui

RESOURCES += \
    image.qrc






