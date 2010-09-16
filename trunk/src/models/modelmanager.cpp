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
        QString tableName = table->name();
        if (tableName.trimmed().isEmpty())
        {
            tableName = getNewDefaultNameForTable();
            table->setName(tableName);
        }
        QMap<QString, SharedTableModel>::const_iterator i = m_tables.constFind(tableName);
        if (i == m_tables.constEnd())
        {
            m_tables.insert(tableName, SharedTableModel(table));
            emit tableAdded(table);
        }
    }
}

void ModelManager::removeTable(const QString& tableName)
{
    QMap<QString, SharedTableModel>::iterator i = m_tables.find(tableName);
    if (i != m_tables.end())
    {
        i.value().clear();
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

const QList<QString> ModelManager::getColumnList(const QString& tableName) const
{
    PTableModel table = getTableByName(tableName);
    QList<QString> rslt;
    if (table)
    {
        rslt = table->columns().keys();
    }
    return rslt;
}

QString ModelManager::getNewDefaultNameForTable() const
{
    QStringList strLst(m_tables.keys());
    QString defaultName = tr("Table");
    QString newName = defaultName;
    if (!strLst.contains(newName, Qt::CaseInsensitive))
    {
        return defaultName;
    }
    else
    {
        for (int i = 1; i < 32000; i++)
        {
            newName = defaultName + "_" + QString::number(i);
            if (!strLst.contains(newName, Qt::CaseInsensitive))
            {
                return newName;
            }
        }
    }
    return "";
}
