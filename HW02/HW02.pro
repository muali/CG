include(../common.pri)

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HW02
TEMPLATE = app

LIBS += -L"C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\x64" -lopengl32

SOURCES += main.cpp\
        mainwindow.cpp \
        glwidget.cpp

HEADERS  += mainwindow.h \
            glwidget.h \
            cubemap.h \
            skybox.h


FORMS    += mainwindow.ui

DISTFILES += bunny_with_normals.obj \
 			 cubemap.frag \
        	 cubemap.vs \
        	 skybox.frag \
        	 skybox.vs