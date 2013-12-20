#-------------------------------------------------
#
# Project created by QtCreator 2013-12-18T16:46:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tiberiusGui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    project.cpp \
    toolchain.cpp

HEADERS  += mainwindow.h \
    project.h \
    toolchain.h

FORMS    += mainwindow.ui

RESOURCES += \
    toolbar.qrc
