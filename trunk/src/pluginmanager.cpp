/***************************************************************************
 *   Copyright (C) 2010 by Pavel Andreev                                   *
 *   Mail: apavelm on gmail point com (apavelm@gmail.com)                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, you can read it                      *
 *   here <http://www.gnu.org/licenses/>, or write to the                  *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "pluginmanager.h"

#include <QtCore/QDir>
#include <QtCore/QStringList>

#include "settingsmanager.h"

PluginManager::PluginManager()
{
}

PluginManager::~PluginManager()
{
    m_dbmsPlugins.clear();
    m_databaseLoaders.clear();
}

PluginDBMSInterface * PluginManager::loadDBMSPlugin(const QString &path)
{
        QPluginLoader * loader = new QPluginLoader(path, qApp);
        if(!loader->load())
        {
            return 0;
        }

        PluginDBMSInterface *plugin = qobject_cast<PluginDBMSInterface *>(loader->instance());
        if(!plugin)
        {
            loader->unload();
            delete loader;
            return 0;
        }

        const QString pluginName = plugin->databaseName();

        if (m_dbDBMSPluginVersions.contains(pluginName))
        {
            int versionExists = m_dbDBMSPluginVersions.value(pluginName);
            int versionNew = plugin->internalVersion();

            // if new plugin version less than already loaded plugin
            if (versionNew <= versionExists)
            {
                loader->unload();
                delete loader;
                return 0;
            }
        }

        m_dbDBMSPluginVersions.insert(pluginName, plugin->internalVersion());
        m_databaseLoaders.insert(pluginName, QSharedPointer<QPluginLoader>(loader));
        m_dbmsPlugins.insert(pluginName, plugin);

        return plugin;
}

void PluginManager::loadDBMSPlugins()
{
    m_dataTypes.clear();
    m_dbDBMSPluginVersions.clear();
    m_dbmsPlugins.clear();
    m_databaseLoaders.clear();

    const QString& path = SM->pluginDirectory();
    QDir plugins_dir(path);

    // We're don't need to process symlinks, only files
    const QStringList files = plugins_dir.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    foreach (QString file, files)
    {
        file.prepend(path);
        PluginDBMSInterface * plugin = loadDBMSPlugin(file);
        if (plugin)
        {
            m_dataTypes.insert(plugin->databaseName(), plugin->dataTypes());
        }
    }
}
