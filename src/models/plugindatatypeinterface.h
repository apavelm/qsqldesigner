#ifndef PLUGINDATATYPEINTERFACE_H
#define PLUGINDATATYPEINTERFACE_H

#include <QtCore/QList>
#include <QtCore/QPluginLoader>
#include <QtCore/QString>

#include "datatypes.h"

class PluginDataTypeInterface
{
public:
    virtual ~PluginDataTypeInterface() {}

    virtual QString databaseName() const = 0;
    virtual QString internalVersion() const = 0;

    virtual QList<DataType> dataTypes() const = 0;
};

Q_DECLARE_INTERFACE(PluginDataTypeInterface, "org.qsqldesigner.plugin.datatype.interface");

typedef QList<QPluginLoader *>    PluginLoaderList;
typedef QList<PluginDataTypeInterface *>  PluginDataTypeList;

#endif // PLUGINDATATYPEINTERFACE_H
