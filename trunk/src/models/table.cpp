#include "table.h"

TableModel::TableModel(const QString& name) : m_name(name)
{

}

void TableModel::setName(const QString& name)
{
    if (m_name != name)
        m_name = name;
};

void TableModel::setColumns(const ColumnList& newColumns)
{
    m_columns = newColumns;
}

void TableModel::addColumn(const ColumnModel& c)
{
    m_columns.addColumn(c);
}
