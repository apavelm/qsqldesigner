#include "table.h"

TableModel::TableModel(const QString& name) : m_name(name)
{

}

void TableModel::setName(const QString& name)
{
    if (m_name != name)
        m_name = name;
};
