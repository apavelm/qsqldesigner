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

void TableModel::addColumn(PColumnModel c)
{
    m_columns.addColumn(c);
}

qreal TableModel::longestStringWidth(const QFontMetrics& metrics) const
{
    qreal maxWidth = 0;
    foreach (SharedColumnModel column, m_columns)
    {
        maxWidth = qMax(maxWidth, (qreal)metrics.boundingRect(column->getUMLColumnDescription()).width());
        foreach (SharedColumnConstraint cn, column->constraints())
        {
            if (cn->type() != ColumnConstraint::CT_Unknown)
            {
                maxWidth = qMax(maxWidth, (qreal)metrics.boundingRect(cn->getUMLConstraintString()).width());
            }
        }
    }

    return maxWidth;
}
