#include "table.h"

#include "modelmanager.h"

TableModel::TableModel(PModelManager mm, const QString& name)
{
    m_mm = mm;
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
    if (m_mm->isTableNameValid(name))
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
    QStringList strLst(m_mm->getTableList());
    QString defaultName = tr("Table");
    QString newName = defaultName;
    int i = 1;
    while (strLst.contains(newName, Qt::CaseInsensitive))
    {
        newName = defaultName + QString("_%1").arg(i++);
    }
    return newName;
}

void TableModel::addConstraint(PConstraint constraint)
{
    // check it adding column-typed constraint
    if ( (constraint->type()  != Constraint::CT_NotNull) && (constraint->type()  != Constraint::CT_Default) )
    {
        m_constraints.addConstraint(constraint);
    }
}
