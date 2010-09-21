#include "column.h"
#include "table.h"

#include <limits>

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
ColumnConstraints::ColumnConstraints() : QList<SharedColumnConstraint>(), m_types(0)
{
}

void ColumnConstraints::addConstraint(PColumnConstraint constraint)
{
    if (constraint)
    {
        if (!m_types.testFlag(constraint->type()))
        {
            append(SharedColumnConstraint(constraint));
            m_types |= constraint->type();
        }
    }
}

void ColumnConstraints::deleteConstraint(int index)
{
    if (index >= count() || index < 0) return;
    m_types ^= at(index)->type();
    removeAt(index);
}

PColumnConstraint ColumnConstraints::constraint(const ColumnConstraint::ConstraintType type) const
{
    if (m_types.testFlag(type))
    {
        foreach (SharedColumnConstraint c, *this)
        {
            if (c->type() == type)
            {
                return c.data();
            }
        }
    }

    return 0;
}

// ColumnModel
ColumnModel::ColumnModel(PTableModel table, const QString& name) : m_parent(table)
{
    if (name.isEmpty())
    {
        m_columnName = defaultColumnName();
    }
    else
    {
        setName(name);
    }

}

void ColumnModel::setName(const QString& name)
{
    if (isValidName(name))
    {
        m_columnName = name;
    }
    else
    {
        // TODO: insert exception
    }
}

void ColumnModel::setComment(const QString& comment)
{
    m_columnComment = comment;
}

void ColumnModel::setDataType(const DataType& dataType)
{
    m_dataType = dataType;
}

const QString ColumnModel::defaultColumnName() const
{
    if (m_parent)
    {
        QStringList strLst(m_parent->columns().keys());
        QString defaultName = tr("Column");
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
    }
    else
        return QString();
}

bool ColumnModel::isValidName(const QString& name) const
{
    if (m_parent)
    {
        // TODO: insert validation check
            return true;
    }
    else
        return false;
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

ColumnList::ColumnList() : QMap<QString, SharedColumnModel>()
{
    qFill(&m_constraintCounters[0], &m_constraintCounters[ColumnConstraint::CT_Last], 0 );
}

void ColumnList::addColumn(PColumnModel column)
{
    if (column)
    {
        foreach (SharedColumnConstraint c, column->constraints())
        {
            m_constraintCounters[c->type()]++;
        }
        insert(column->name(), SharedColumnModel(column));
    }
}

PColumnModel ColumnList::getColumnByName(const QString& columnName) const
{
    QMap<QString, SharedColumnModel>::const_iterator i = this->constFind(columnName);
    if (i != this->constEnd())
    {
        return i.value().data();
    }
    else
        return 0;
}

void ColumnList::getColumnsForConstraintType(const ColumnConstraint::ConstraintType type, QList<PColumnModel>& result) const
{
    result.clear();
    if (m_constraintCounters[type] > 0)
    {
        foreach (SharedColumnModel c, *this)
        {
            if (c->isConstraintType(type))
                result.append(c.data());
        }
    }
}
