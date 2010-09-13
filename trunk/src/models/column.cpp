#include "column.h"

// ColumnConstraint
ColumnConstraint::ColumnConstraint(const QString name, const ConstraintType type, const QVariant& data): m_type(type)
{
    m_name = name;
    m_data = data;
}

void ColumnConstraint::setName(const QString& newName)
{
    // TODO: insert name validation check
    m_name = newName;
}

void ColumnConstraint::setData(const QVariant& newData)
{
    if (newData.isValid())
        m_data = newData;
}

const QString ColumnConstraint::getUMLConstraintString() const
{
    QString rslt;
    // template: "+   NAME(datatype)"
    switch (m_type)
    {
        case CT_PrimaryKey: break;
        case CT_NotNull: break;
        case CT_Unique: break;
        case CT_ForeignKey: break;
        case CT_Default: break;
        case CT_Check: break;
        default: break;
    }
    rslt = m_name + "()";
    return rslt;
}

// ColumnConstraints
ColumnConstraints::ColumnConstraints() : QList<ColumnConstraint>(), m_types(0)
{
}

void ColumnConstraints::addConstraint(ColumnConstraint * constraint)
{
    if (constraint)
    {
        if (!m_types.testFlag(constraint->type()))
        {
            append(*constraint);
            m_types |= constraint->type();
            delete constraint;
        }
    }
}

void ColumnConstraints::deleteConstraint(int index)
{
    if (index >= count() || index < 0) return;
    m_types ^= at(index).type();
    removeAt(index);
}

// ColumnModel
void ColumnModel::setName(const QString& name)
{
    m_columnName = name;
}

void ColumnModel::setComment(const QString& comment)
{
    m_columnComment = comment;
}

#include <QDebug>
const QString ColumnModel::getUMLColumnPrefix() const
{
    qDebug() << m_columnName;
    qDebug() << m_constraints.types();

    QString rslt = "";
    if (isNotNull())
    {
        rslt += "*";
    }
    if (isPrimaryKey() && isForeignKey())
    {
        rslt+="pfK";
    }
    else
    {
        if (isPrimaryKey())
        {
            rslt += "PK";
        }
        if (isForeignKey())
        {
            rslt += "FK";
        }
    }
    return rslt;
}

const QString ColumnModel::getUMLColumnDescription() const
{
    QString rslt = m_columnName + ": " + "datatype";
    return rslt;
}

// Column List

ColumnList::ColumnList() : QList<ColumnModel>()
{
    for (int i = 0; i < ColumnConstraint::CT_Last; i++)
        m_constraintCounters[i] = 0;
}

void ColumnList::addColumn(const ColumnModel& column)
{
    if (column.isPrimaryKey())
        m_constraintCounters[ColumnConstraint::CT_PrimaryKey]++;
    if (column.isNotNull())
        m_constraintCounters[ColumnConstraint::CT_NotNull]++;
    if (column.isUnique())
        m_constraintCounters[ColumnConstraint::CT_Unique]++;
    if (column.isCheckConstraint())
        m_constraintCounters[ColumnConstraint::CT_Check]++;
    if (column.isForeignKey())
        m_constraintCounters[ColumnConstraint::CT_ForeignKey]++;
    append(column);
}
