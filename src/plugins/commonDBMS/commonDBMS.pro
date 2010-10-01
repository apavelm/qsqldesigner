TARGET = bin/plugins/commonDBMS
DESTDIR = ../out
TEMPLATE = lib
INCLUDEPATH += ../../models/
DEFINES += QSQLDESIGNER_DBMS_PLUGIN_LIBRARY
SOURCES += commondbms.cpp \
    ../../models/modelmanager.cpp \
    ../../models/table.cpp \
    ../../models/column.cpp \
    ../../models/constraint.cpp
HEADERS += commondbms.h \
    plugindbmsinterface.h \
    modelmanager.h \
    table.h \
    datatypes.h \
    column.h \
    constraint.h \
    ../../sqldesignerproject.h
