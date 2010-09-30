#ifndef PLUGINDATATYPEINTERFACE_H
#define PLUGINDATATYPEINTERFACE_H

#include <QtCore/QPluginLoader>
#include <QtCore/QString>

#include "datatypes.h"

class PluginDataTypeInterface
{
public:
    virtual ~PluginDataTypeInterface() {}

    virtual const QString databaseName() const = 0;
    virtual int internalVersion() const = 0;

    virtual const DataTypes& dataTypes() const = 0;
};

Q_DECLARE_INTERFACE(PluginDataTypeInterface, "org.qsqldesigner.plugin.datatype.interface");

#endif // PLUGINDATATYPEINTERFACE_H
