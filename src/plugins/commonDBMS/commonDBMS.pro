TARGET = bin/plugins/commonDBMS
QT += core
DESTDIR = ../out
TEMPLATE = lib
DEFINES += QSQLDESIGNER_DBMS_PLUGIN_LIBRARY
SOURCES += commondbms.cpp \
    ../../sqldesignerprojectsettings.cpp \
    ../../models/datatypes.cpp \
    ../../models/modelmanager.cpp \
    ../../models/table.cpp \
    ../../models/column.cpp \
    ../../models/constraint.cpp
HEADERS += commondbms.h \
    ../../sqldesignerprojectsettings.h \
    ../../models/plugindbmsinterface.h \
    ../../models/modelmanager.h \
    ../../models/table.h \
    ../../models/datatypes.h \
    ../../models/column.h \
    ../../models/constraint.h
