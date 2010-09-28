#include "table.h"

#include <limits>
#include "modelmanager.h"

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

void TableModel::setColumns(const ColumnList& newColumns)
{
    m_columns = newColumns;
}

void TableModel::addColumn(PColumnModel c)
{
    m_columns.addColumn(c);
}

void TableModel::removeColumn(const QString& columnName)
{
    m_columns.remove(columnName);
}

const QStringList TableModel::constraintsNames() const
{
    QStringList rsltLst;

    // adding column's constraints
    foreach (const SharedColumnModel& c, m_columns)
    {
        const Constraints cns = c->constraints();
        foreach (const SharedConstraint& cn, cns)
        {
            if (!cn->name().isEmpty())
            {
                rsltLst.append(cn->name());
            }
        }
    }

    // adding table's constraints
    foreach (const SharedConstraint& cn, m_constraints)
    {
        if (!cn->name().isEmpty())
        {
            rsltLst.append(cn->name());
        }
    }

    return rsltLst;
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
    return QString();
}

void TableModel::addConstraint(PConstraint constraint)
{
    // check it adding column-typed constraint
    if ( (constraint->type()  != Constraint::CT_NotNull) && (constraint->type()  != Constraint::CT_Default) )
    {
        m_constraints.addConstraint(constraint);
    }
}

bool TableModel::isValidName(const QString& name) const
{
    return MM->isTableNameValid(name);
}
