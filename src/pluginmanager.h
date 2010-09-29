#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QtCore/QMap>
#include <QtCore/QPluginLoader>
#include <QtCore/QScopedPointer>
#include <QtCore/QString>

#include "singleton.h"
#include "models/plugindatatypeinterface.h"

class PluginManager: public Singleton<PluginManager>
{
public:
        // Datatypes
        void loadDataTypePlugins();
        DataTypes dataTypesForDataBase(const QString& dbName) const {return m_dataTypes.value(dbName);}

private:
        friend class Singleton<PluginManager>;
        PluginManager();
        virtual ~PluginManager();

        // Datatypes
        PluginDataTypeInterface * loadDataTypePlugin(const QString& path);
        QScopedPointer<QPluginLoader> m_dataTypeLoader;
        AllDatabaseDataTypes m_dataTypes;
        QMap<QString, int> m_dbDatatypesPluginVersions;
};

#define PLUGINMANAGER PluginManager::getInstance()

#endif // PLUGINMANAGER_H
