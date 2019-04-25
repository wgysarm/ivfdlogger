#-------------------------------------------------
#
# Project created by QtCreator 2018-01-03T13:43:51
#
#-------------------------------------------------

QT       -= gui

TARGET = IvfdLogger
TEMPLATE = lib

DEFINES += IVFDLOGGER_LIBRARY

SOURCES += IvfdLogger.cpp

HEADERS += IvfdLogger.h\
        ivfdlogger_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
