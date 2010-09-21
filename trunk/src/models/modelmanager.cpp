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
        m_tables.insert(table->name(), SharedTableModel(table));
        emit tableAdded(table);
    }
}

void ModelManager::removeTable(const QString& tableName)
{
    if (m_tables.contains(tableName))
    {
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

const QList<QString> ModelManager::getTableListExceptOne(const QString& tableName) const
{
    QMap<QString, SharedTableModel>::const_iterator i = m_tables.constFind(tableName);
    if (i == m_tables.constEnd())
    {
        return m_tables.keys();
    }
    else
    {
        QList<QString> lst(m_tables.keys());
        int idx = lst.indexOf(tableName);
        lst.removeAt(idx);
        return lst;
    }
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
