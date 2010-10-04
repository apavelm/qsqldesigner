# -------------------------------------------------
# Project created by QtCreator 2009-05-15T22:05:17
# -------------------------------------------------
QT += sql \
	xml \
	svg
TARGET = bin/qsqldesigner
DESTDIR = out
TEMPLATE = app
SOURCES += src/main.cpp \
    src/mainwindow.cpp \
	src/columndialog.cpp \
	src/magnifydialog.cpp \
	src/tabledialog.cpp \
	src/commands.cpp \
	src/widgets/mainview.cpp \
	src/widgets/tablewidget.cpp \
	src/widgets/cornerzoomwidget.cpp \
	src/models/column.cpp \
	src/models/modelmanager.cpp \
	src/models/table.cpp \
    src/settingsmanager.cpp \
    src/simpleforeignkeyselectdialog.cpp \
    src/simpleforeignkeyselectionviewmodel.cpp \
    src/widgets/columnpreviewwidget.cpp \
    src/widgets/widgetmanager.cpp \
    src/models/constraint.cpp \
    src/widgets/arrowforeignkey.cpp \
    src/pluginmanager.cpp \
    src/projectmanager.cpp \
    src/sqldesignerproject.cpp \
    src/newprojectdialog.cpp \
    src/aboutdialog.cpp \
    src/sqldesignerprojectsettings.cpp \
    src/widgets/objecteditor.cpp \
    src/widgets/widgetviewmodel.cpp
HEADERS += src/mainwindow.h \
	src/columndialog.h \
	src/magnifydialog.h \
	src/tabledialog.h \
	src/commands.h \
	src/widgets/mainview.h \
	src/widgets/tablewidget.h \
	src/widgets/cornerzoomwidget.h \
	src/models/column.h \
	src/models/datatypes.h \
	src/models/modelmanager.h \
	src/models/table.h \
    src/settingsmanager.h \
    src/singleton.h \
    src/simpleforeignkeyselectdialog.h \
    src/simpleforeignkeyselectionviewmodel.h \
    src/widgets/columnpreviewwidget.h \
    src/widgets/widgetmanager.h \
    src/models/constraint.h \
    src/widgets/arrowforeignkey.h \
    src/models/plugindbmsinterface.h \
    src/pluginmanager.h \
    src/projectmanager.h \
    src/sqldesignerproject.h \
    src/newprojectdialog.h \
    src/aboutdialog.h \
    src/sqldesignerprojectsettings.h \
    src/widgets/objecteditor.h \
    src/widgets/widgetviewmodel.h
FORMS += src/ui/mainwindow.ui \
    src/ui/columndialog.ui \
    src/ui/magnifydialog.ui \
    src/ui/tabledialog.ui \
    src/ui/simpleforeignkeyselectdialog.ui \
    src/ui/columnpreviewwidget.ui \
    src/ui/newprojectdialog.ui \
    src/ui/aboutdialog.ui \
    src/ui/objecteditor.ui
RESOURCES += resources/main.qrc
