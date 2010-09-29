#ifndef COMMONDATATYPESPLUGIN_H
#define COMMONDATATYPESPLUGIN_H

#include <QtCore/QtPlugin>

#include "plugindatatypeinterface.h"
#include "datatypes.h"


class CommonDataTypesPlugin : public QObject, public PluginDataTypeInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginDataTypeInterface)
public:
    CommonDataTypesPlugin();

    inline const QString databaseName() const { return "Common Database"; }
    inline int internalVersion() const { return 1; }
    inline const DataTypes& dataTypes() const {return m_dataypeList;}
private:
    DataTypes m_dataypeList;
};

#endif // COMMONDATATYPESPLUGIN_H
