#include "table.h"
#include "modelmanager.h"

#include <limits>

TableModel::TableModel(const QString& name)
{
    if (name.isEmpty())
    {
        m_name = defaultTableName();
    }
    else
    {
        setName(name);
    }
}

void TableModel::setName(const QString& name)
{
    if (isValidName(name))
    {
        m_name = name;
    }
};

bool TableModel::isValidName(const QString& name) const
{
    // TODO: insert validation check
    return true;
}

void TableModel::setColumns(const ColumnList& newColumns)
{
    m_columns = newColumns;
}

void TableModel::addColumn(PColumnModel c)
{
    m_columns.addColumn(c);
}

const QString TableModel::defaultTableName() const
{
    QStringList strLst(MM->getTableList());
    QString defaultName = tr("Table");
    QString newName = defaultName;
    if (!strLst.contains(newName, Qt::CaseInsensitive))
    {
        return defaultName;
    }
    else
    {
        for (int i = 1; i < std::numeric_limits<int>::max(); i++)
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
