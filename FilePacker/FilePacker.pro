#-------------------------------------------------
#
# Project created by QtCreator 2016-01-04T08:22:29
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11

TARGET = FilePacker

DESTDIR = ../../../../Lib

TEMPLATE = lib
CONFIG += staticlib

SOURCES += filepacker.cpp

HEADERS += filepacker.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

DESTDIR = $$PWD/../../../../Main/Lib

