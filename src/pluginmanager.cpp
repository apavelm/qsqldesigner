#include "pluginmanager.h"

#include <QtDebug>

PluginManager::PluginManager() : m_dataTypeLoader(0)
{
}

PluginManager::~PluginManager()
{
}

PluginDataTypeInterface * PluginManager::loadDataTypePlugin(const QString &path)
{
    if (m_dataTypeLoader.isNull())
    {
        m_dataTypeLoader.reset(new QPluginLoader(path));
        if(!m_dataTypeLoader->load())
        {
            return 0;
        }

        PluginDataTypeInterface *plugin = qobject_cast<PluginDataTypeInterface *>(m_dataTypeLoader->instance());
        if(!plugin)
        {
            m_dataTypeLoader->unload();
            m_dataTypeLoader.reset();
            return 0;
        }

        return plugin;
    }
    else
        return 0;
}

void PluginManager::loadDataTypePlugins()
{
    m_dataTypes.clear();
    m_dbDatatypesPluginVersions.clear();
    QString path = qApp->applicationDirPath() + "/plugins/";
    QDir plugins_dir(path);

    // We're don't need to process symlinks, only files
    QStringList files = plugins_dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    for(int i = 0; i < files.count(); ++i)
    {
        QString filename = files[i].prepend(path);
        PluginDataTypeInterface * plugin = loadDataTypePlugin(filename);
        if (plugin)
        {
            // search if plugin for this DB type is already loaded
            QMap<QString, int>::const_iterator iterator = m_dbDatatypesPluginVersions.constFind(plugin->databaseName());
            if (iterator != m_dbDatatypesPluginVersions.constEnd())
            {
                // if new plugin version more than already loaded plugin
                if (*iterator < plugin->internalVersion())
                {
                    m_dbDatatypesPluginVersions.insert(plugin->databaseName(), plugin->internalVersion());
                    m_dataTypes.insert(plugin->databaseName(), plugin->dataTypes());
                }
            }
            else
            {
                // if there is no such plugin loaded
                m_dbDatatypesPluginVersions.insert(plugin->databaseName(), plugin->internalVersion());
                m_dataTypes.insert(plugin->databaseName(), plugin->dataTypes());
            }

            // unload plugin
            if (!m_dataTypeLoader.isNull())
            {
                m_dataTypeLoader->unload();
                m_dataTypeLoader.reset();
            }
        }
    }
}
