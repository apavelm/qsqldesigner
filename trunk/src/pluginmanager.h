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

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QtCore/QMap>
#include <QtCore/QPluginLoader>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>

#include "singleton.h"
#include "models/plugindbmsinterface.h"

class PluginManager: public Singleton<PluginManager>
{
public:
        void loadDBMSPlugins();

        inline DataTypes dataTypesForDatabase(const QString& dbName) const {return m_dataTypes.value(dbName);}
        PluginDBMSInterface * pluginForDatabase(const QString& dbName) const {return m_dbmsPlugins.value(dbName);}
        inline QList<QString> dbmsList() const {return m_dbmsPlugins.keys();}

private:
        friend class Singleton<PluginManager>;
        PluginManager();
        virtual ~PluginManager();

        PluginDBMSInterface * loadDBMSPlugin(const QString& path);

        QMap<QString, QSharedPointer<QPluginLoader> > m_databaseLoaders;
        QMap<QString, PluginDBMSInterface *> m_dbmsPlugins;
        QMap<QString, int> m_dbDBMSPluginVersions;
        AllDatabaseDataTypes m_dataTypes;
};

#define PLUGINMANAGER PluginManager::getInstance()

#endif // PLUGINMANAGER_H
