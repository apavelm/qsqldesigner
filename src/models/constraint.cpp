#include "constraint.h"

// Constraint
Constraint::Constraint(const ConstraintType type, const QVariant& data): m_type(type), m_name(QString())
{
    m_data = data;
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
    // TODO: insert name validation check
    m_name = newName;
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

const QString Constraint::getUMLConstraintString() const
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
