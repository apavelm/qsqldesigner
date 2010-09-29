TARGET = bin/plugins/baseTypes
DESTDIR = ../out
TEMPLATE = lib
INCLUDEPATH += ../../models
DEFINES += QSQLDESIGNER_DATATYPE_PLUGIN_LIBRARY
SOURCES += commondatatypes.cpp
HEADERS += commondatatypes.h \
    plugindatatypeinterface.h
