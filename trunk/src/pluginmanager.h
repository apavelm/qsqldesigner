#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QList>
#include <QtCore/QPluginLoader>
#include <QtCore/QString>

#include "singleton.h"
#include "models/plugindatatypeinterface.h"

class PluginManager: public Singleton<PluginManager>
{
public:
        PluginDataTypeInterface * loadPlugin(const QString& path);
        bool unloadPlugin(PluginDataTypeInterface *object);
        void loadPlugins();
        inline int pluginsCount() const {return m_loaders.count();}

        PluginDataTypeInterface * getDataTypePlugin(const QString& databaseName) const;
        inline PluginDataTypeList & getPluginList() { return m_plugins; }

private:
        friend class Singleton<PluginManager>;
        PluginManager();
        virtual ~PluginManager();

        PluginLoaderList m_loaders;
        PluginDataTypeList m_plugins;
};

#define PLUGINMANAGER PluginManager::getInstance()

#endif // PLUGINMANAGER_H
