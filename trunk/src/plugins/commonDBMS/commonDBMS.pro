TARGET = bin/plugins/commonDBMS
DESTDIR = ../out
TEMPLATE = lib
INCLUDEPATH += ../../models
DEFINES += QSQLDESIGNER_DBMS_PLUGIN_LIBRARY
SOURCES += commondbms.cpp
HEADERS += commondbms.h \
    plugindbmsinterface.h
