#include "column.h"
#include "table.h"

#include <limits>

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
    if (isConstraintType(Constraint::CT_NotNull))
    {
        rslt += "*";
    }
    if (isConstraintType(Constraint::CT_PrimaryKey) && isConstraintType(Constraint::CT_ForeignKey))
    {
        rslt+="pfK";
    }
    else
    {
        if (isConstraintType(Constraint::CT_PrimaryKey))
        {
            rslt += "PK";
        }
        if (isConstraintType(Constraint::CT_ForeignKey))
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
    qFill(&m_constraintCounters[0], &m_constraintCounters[Constraint::CT_Last], 0 );
}

void ColumnList::addColumn(PColumnModel column)
{
    if (column)
    {
        foreach (const SharedConstraint& c, column->constraints())
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

void ColumnList::getColumnsForConstraintType(const Constraint::ConstraintType type, QList<PColumnModel>& result) const
{
    result.clear();
    if (m_constraintCounters[type] > 0)
    {
        foreach (const SharedColumnModel& c, *this)
        {
            if (c->isConstraintType(type))
                result.append(c.data());
        }
    }
}
