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

#include "modelmanager.h"

#include "../sqldesignerproject.h"

ModelManager::ModelManager(QObject * parent) : QObject(parent)
{
    m_project = qobject_cast<PSqlDesignerProject>(parent);
}

void ModelManager::addTable(PTableModel table)
{
    if (table)
    {
        // add table
        m_tables.insert(table->name(), SharedTableModel(table));
        // add table's and it's column's constraints
        m_constraintNames << table->constraintsNames();
        emit tableAdded(table);
    }
}

void ModelManager::removeTable(const QString& tableName)
{
    if (m_tables.contains(tableName))
    {
        // removing table's and it's column's constraints
        PTableModel table = getTableByName(tableName);
        const QStringList tablesConstraintNames = table->constraintsNames();
        foreach (const QString& item, tablesConstraintNames)
        {
            m_constraintNames.removeOne(item);
        }
        // removing table
        m_tables.remove(tableName);
        emit tableRemoved(tableName);
    }
}

PTableModel ModelManager::getTableByName(const QString& tableName) const
{
    QMap<QString, SharedTableModel>::const_iterator i = m_tables.constFind(tableName);
    if (i != m_tables.constEnd())
    {
        return i.value().data();
    }
    return 0;
}

PColumnModel ModelManager::getColumnByName(const QString& tableName, const QString& columnName) const
{
    PTableModel table = getTableByName(tableName);
    if (table)
    {
        return table->columns().getColumnByName(columnName);
    }
    else
        return 0;
}

// TODO: add filtering by datatype
const QList<QString> ModelManager::getColumnList(const QString &tableName, const DataType) const
{
    PTableModel table = getTableByName(tableName);
    QList<QString> rslt;
    if (table)
    {
        rslt = table->columns().keys();
    }
    return rslt;
}

bool ModelManager::isTableNameValid(const QString& tableName) const
{
    QStringList lst(m_tables.keys());
    if (!lst.contains(tableName, Qt::CaseInsensitive))
    {
        return true;
    }
    else
    {
        // TODO: insert ACTION if wrong name detected after validation check
        return false;
    }
}

bool ModelManager::isConstraintNameValid(const QString& name) const
{
    if (!m_constraintNames.contains(name, Qt::CaseInsensitive))
    {
        return true;
    }
    else
    {
        // TODO: insert ACTION if wrong name detected after validation check
        return false;
    }
}
