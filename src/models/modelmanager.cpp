#include "modelmanager.h"

ModelManager::ModelManager()
{

}


ModelManager::~ModelManager()
{

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
