#include "constraint.h"
#include "column.h"
#include "modelmanager.h"
#include "table.h"

#include <limits>

// Constraint
Constraint::Constraint(PColumnModel column, const ConstraintType type, const QVariant& data) : m_column(column), m_type(type)
{
    m_data = data;
    m_name = defaultName(type, data);
}

Constraint::Constraint(const Constraint& old)
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

Constraint::~Constraint()
{
    m_data.clear();
}

void Constraint::setName(const QString& newName)
{
    // if necessary to clear name, do it w/o any checks
    if (newName.isEmpty())
    {
        m_name = newName;
        return;
    }

    // name validation check
    if (MM->isConstraintNameValid(newName))
    {
        m_name = newName;
    }
    else
    {
        // TODO: insert ACTION after exception
    }
}

void Constraint::setType(const ConstraintType& newType)
{
    m_type = newType;
}

void Constraint::setData(const QVariant& newData)
{
    if (newData.isValid())
        m_data = newData;
}

QString Constraint::defaultName(const ConstraintType type, const QVariant& var)
{
    QString rslt = QString();
    if (type == Constraint::CT_ForeignKey)
    {
        if (var.canConvert<ConstraintForeignKey>())
        {
            ConstraintForeignKey fk = var.value<ConstraintForeignKey>();
            rslt = QString("FK_%1_%2_REF_%3_%4").arg(m_column->table()->name()).arg(m_column->name()).arg(fk.referenceTable()).arg(fk.referenceColumns().first());
        }
    }
    if (type == Constraint::CT_PrimaryKey)
    {
        rslt = QString("PK_%1").arg(m_column->table()->name());
    }
    if (type == Constraint::CT_Unique)
    {
        rslt = QString("UQ_%1_%2").arg(m_column->table()->name()).arg(m_column->name());
    }

    //checking for duplicate
    if (!rslt.isEmpty())
    {
        QString newName;
        if (!MM->isConstraintNameValid(rslt))
        {
            for (int i = 1; i < std::numeric_limits<int>::max(); i++)
            {
                newName = rslt + "_" + QString::number(i);
                if (MM->isConstraintNameValid(newName))
                {
                    return newName;
                }
            }
        }
    }

    // returning value
    return rslt;
}

const QString Constraint::getUMLConstraintString() const
{
    QString rslt;
    // template: "+   NAME(datatype)"
    switch (m_type)
    {
        case CT_PrimaryKey:
            {
                if (!m_name.isEmpty())
                {
                    QString sTypeListString;
                    QList<PColumnModel> lstPrimaryKeys;
                    m_column->table()->columns().getColumnsForConstraintType(Constraint::CT_PrimaryKey, lstPrimaryKeys);
                    foreach (const PColumnModel c, lstPrimaryKeys)
                    {
                        sTypeListString += ( sTypeListString.isEmpty() ? "(" : ", ");
                        sTypeListString += c->dataType().typeName;
                    }
                    sTypeListString += ")";
                    rslt = m_name + sTypeListString;
                }
            }
            break;
        case CT_NotNull: break;
        case CT_Unique:
            {
                rslt = QString("%1(%2)").arg(m_name).arg(m_column->dataType().typeName);
            }
            break;
        case CT_ForeignKey:
            {
                // WRONG!
                // TODO: if 2 or more FK references to same refTable -> than it is 1 string in FK-constraint else (datatype)

                /*if (!m_name.isEmpty())
                {
                    QString refTableName;
                    QVariant var;
                    if (var.canConvert<ConstraintForeignKey>())
                    {
                        ConstraintForeignKey fk = var.value<ConstraintForeignKey>();
                        refTableName = fk.referenceTable();
                    }
                    QString sTypeListString;
                    QList<PColumnModel> lstForeignKeys;
                    m_column->table()->columns().getColumnsForConstraintType(Constraint::CT_ForeignKey, lstForeignKeys);
                    foreach (const PColumnModel c, lstForeignKeys)
                    {
                        sTypeListString += ( sTypeListString.isEmpty() ? "(" : ", ");
                        sTypeListString += c->dataType().typeName;
                    }
                    sTypeListString += ")";

                    rslt = m_name + sTypeListString;
                }*/
                // DELME: temporary
                rslt = m_name + "()";
            }
            break;
        case CT_Default: break;
        case CT_Check: break;
        default: break;
    }
    return rslt;
}

// Constraints
Constraints::Constraints() : QList<SharedConstraint>(), m_types(0)
{
}

void Constraints::addConstraint(PConstraint constraint)
{
    if (constraint)
    {
        if (!m_types.testFlag(constraint->type()))
        {
            append(SharedConstraint(constraint));
            m_types |= constraint->type();
        }
    }
}

void Constraints::deleteConstraint(int index)
{
    if (index >= count() || index < 0) return;
    m_types ^= at(index)->type();
    removeAt(index);
}

void Constraints::deleteConstraint(PConstraint constraint)
{
    for (int i = 0; i < count(); i++)
    {
        if (at(i).data() == constraint)
            deleteConstraint(i);
    }
}

PConstraint Constraints::constraint(const Constraint::ConstraintType type) const
{
    if (m_types.testFlag(type))
    {
        foreach (const SharedConstraint& c, *this)
        {
            if (c->type() == type)
            {
                return c.data();
            }
        }
    }

    return 0;
}
