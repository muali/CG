#-------------------------------------------------
#
# Project created by QtCreator 2015-10-30T03:34:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HW01
TEMPLATE = app

LIBS += -L"C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\x64" -lopengl32

SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp

HEADERS  += mainwindow.h \
    glwidget.h

FORMS    += mainwindow.ui

DISTFILES += \
    make_vc.bat \
    mandelbrot.frag
