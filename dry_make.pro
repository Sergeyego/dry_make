#-------------------------------------------------
#
# Project created by QtCreator 2016-12-23T15:55:52
#
#-------------------------------------------------

QT       += core gui sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dry_make
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pgdialog.cpp \
    modeleldim.cpp \
    cbrelationdelegate.cpp \
    dbtablemodel.cpp \
    viewer.cpp \
    modelcont.cpp \
    modeloven.cpp \
    modelprogovn.cpp

HEADERS  += mainwindow.h \
    pgdialog.h \
    modeleldim.h \
    cbrelationdelegate.h \
    dbtablemodel.h \
    viewer.h \
    modelcont.h \
    modeloven.h \
    modelprogovn.h

FORMS    += mainwindow.ui \
    pgdialog.ui

RESOURCES += \
    res.qrc
