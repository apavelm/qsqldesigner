#include "pluginmanager.h"

PluginManager::PluginManager()
{
}

PluginManager::~PluginManager()
{
/*    foreach(PluginInterface *plugin, m_plugins)
    {
        if(plugin) plugin->release();
    }

    qDeleteAll(m_loaders);
    */
    m_plugins.clear();
    m_loaders.clear();
}

PluginDataTypeInterface * PluginManager::loadPlugin(const QString &path)
{
    QPluginLoader *loader = new QPluginLoader(path);
    if(!loader->load())
    {
        return 0;
    }

    PluginDataTypeInterface *plugin = qobject_cast<PluginDataTypeInterface *>(loader->instance());
    if(!plugin)
    {
        loader->unload();
        delete loader;
        return 0;
    }


    m_plugins.append(plugin);
    m_loaders.append(loader);

    return plugin;
}

bool PluginManager::unloadPlugin(PluginDataTypeInterface *object)
{
    for(int i = 0; i < m_loaders.count(); ++i)
    {
        QPluginLoader *loader = m_loaders[i];
        PluginDataTypeInterface *plugin = qobject_cast<PluginDataTypeInterface *>(loader->instance());
        if(plugin == object) {
            //plugin->release();
            delete loader;
            m_loaders.removeAt(i);
            m_plugins.removeAll(plugin);
            return true;
        }
    }

    return false;
}

void PluginManager::loadPlugins()
{
    QString path = qApp->applicationDirPath() + "/plugins/";
    QDir plugins_dir(path);

    // We're don't need to process symlinks, only files
    QStringList files = plugins_dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    for(int i = 0; i < files.count(); ++i)
    {
        QString filename = files[i].prepend(path);
        PluginDataTypeInterface * plugin = loadPlugin(filename);
        if (!plugin) continue;
    }
}

PluginDataTypeInterface * PluginManager::getDataTypePlugin(const QString& databaseName) const
{
    PluginDataTypeInterface * result = 0;

    foreach(PluginDataTypeInterface * p, m_plugins)
    {
        if (QString::compare(databaseName, p->databaseName(), Qt::CaseInsensitive) == 0 )
        {
            result = p;
            break;
        }
    }

    return result;
}
