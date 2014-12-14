#-------------------------------------------------
#
# Project created by QtCreator 2014-12-14T13:26:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = trasy_biegaczy
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    generator.cpp \
    readgraph.cpp \
    graph.cpp

HEADERS  += mainwindow.h \
    generator.h \
    readgraph.h \
    graph.h

FORMS    += mainwindow.ui
