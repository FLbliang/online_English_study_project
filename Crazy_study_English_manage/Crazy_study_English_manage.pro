#-------------------------------------------------
#
# Project created by QtCreator 2017-12-24T22:15:36
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Crazy_study_English_manage
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    errordialog.cpp \
    loginwindow.cpp \
    configure.cpp \
    basewindow.cpp \
    managewindow.cpp \
    listwindow.cpp \
    tablewindow.cpp \
    datashowwindow.cpp \
    dataexplain.cpp \
    socketcontroller.cpp \
    maincontroller.cpp \
    filetable.cpp \
    tablebutton.cpp

HEADERS  += widget.h \
    errordialog.h \
    loginwindow.h \
    configure.h \
    basewindow.h \
    managewindow.h \
    listwindow.h \
    tablewindow.h \
    datashowwindow.h \
    dataexplain.h \
    socketcontroller.h \
    maincontroller.h \
    filetable.h \
    tablebutton.h

FORMS    += widget.ui

CONFIG += mobility
MOBILITY = 

