#include "column.h"

// ColumnConstraint
ColumnConstraint::ColumnConstraint(const QString name, const ConstraintType type, const QVariant& data): m_type(type)
{
    m_name = name;
    m_data = data;
}

ColumnConstraint::ColumnConstraint(const ColumnConstraint& old)
{
    m_name = old.name();
    m_type = old.type();
    if (m_data.isValid())
    {
        m_data = old.data();
    }
    else
    {
        m_data = QVariant();
    }
}

ColumnConstraint::~ColumnConstraint()
{
    m_data.clear();
}

void ColumnConstraint::setName(const QString& newName)
{
    // TODO: insert name validation check
    m_name = newName;
}

void ColumnConstraint::setType(const ConstraintType& newType)
{
    m_type = newType;
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

void ColumnConstraints::constraint(const ColumnConstraint::ConstraintType type, ColumnConstraint& result) const
{
    if (m_types.testFlag(type))
    {
        foreach (const ColumnConstraint& c, *this)
        {
            if (c.type() == type)
            {
                result = c;
            }
        }
    }
    else
        result.setType(ColumnConstraint::CT_Unknown);
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

const QString ColumnModel::getUMLColumnPrefix() const
{
    QString rslt = "";
    if (isConstraintType(ColumnConstraint::CT_NotNull))
    {
        rslt += "*";
    }
    if (isConstraintType(ColumnConstraint::CT_PrimaryKey) && isConstraintType(ColumnConstraint::CT_ForeignKey))
    {
        rslt+="pfK";
    }
    else
    {
        if (isConstraintType(ColumnConstraint::CT_PrimaryKey))
        {
            rslt += "PK";
        }
        if (isConstraintType(ColumnConstraint::CT_ForeignKey))
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
    qFill(&m_constraintCounters[0], &m_constraintCounters[ColumnConstraint::CT_Last], 0 );
}

void ColumnList::addColumn(const ColumnModel& column)
{
    const ColumnConstraints& list = column.constraints();
    foreach (const ColumnConstraint& c, list)
    {
        m_constraintCounters[c.type()]++;
    }
    append(column);
}

void ColumnList::getColumnsForConstraintType(const ColumnConstraint::ConstraintType type, QList<ColumnModel>& result) const
{
    result.clear();
    if (m_constraintCounters[type] > 0)
    {
        const ColumnList& list = *this;
        foreach (const ColumnModel& c, list)
        {
            if (c.isConstraintType(type))
                result.append(c);
        }
    }
}
