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
#    generator.cpp \
#    readgraph.cpp \
    graph.cpp \
    graphview.cpp \
    trasa.cpp

HEADERS  += mainwindow.h \
#    generator.h \
#    readgraph.h \
    graph.h \
    graphview.h \
    trasa.h

FORMS    += mainwindow.ui\
        graphview.ui \
    graphview.ui
