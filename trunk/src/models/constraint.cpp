/***************************************************************************
 *   Copyright (C) 2010 by Pavel Andreev                                   *
 *   Mail: apavelm on gmail point com (apavelm@gmail.com)                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, you can read it                      *
 *   here <http://www.gnu.org/licenses/>, or write to the                  *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "constraint.h"
#include "column.h"
#include "modelmanager.h"
#include "table.h"

// Constraint
Constraint::Constraint(PColumnModel column, const ConstraintType type, const QVariant& data) : m_column(column), m_type(type)
{
    m_data = data;
    m_name = defaultName(type, data);
}

Constraint::~Constraint()
{
    m_data.clear();
}

void Constraint::setName(const QString& newName)
{
    // if necessary to clear name, do it w/o any checks
    if (newName.isEmpty() || m_column->table()->modelManager()->isConstraintNameValid(newName))
    {
        m_name = newName;
        return;
    }

    // TODO: insert ACTION after exception
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
        QString newName = rslt;
        int i = 1;
        while (!m_column->table()->modelManager()->isConstraintNameValid(rslt))
        {
            newName = rslt + "_" + QString::number(i++);
        }
        return newName;
    }

    // returning value
    return rslt;
}

const QString Constraint::getUMLConstraintString() const
{
    QString rslt;

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
                        sTypeListString += c->dataType()->typeName();
                    }
                    sTypeListString += ")";
                    rslt = m_name + sTypeListString;
                }
            }
            break;
        case CT_NotNull: break;
        case CT_Unique:
            {
                rslt = QString("%1(%2)").arg(m_name).arg(m_column->dataType()->typeName());
            }
            break;
        case CT_ForeignKey:
            {
                if (!m_name.isEmpty())
                {
                    QStringList sTypes;
                    QVariant var = m_data;
                    if (var.canConvert<ConstraintForeignKey>())
                    {
                        ConstraintForeignKey fk = var.value<ConstraintForeignKey>();
                        const QList<QString> cols = fk.sourceColumns();
                        foreach (const QString& c, cols)
                        {
                            PColumnModel pColumn = m_column->table()->column(c);
                            if (pColumn)
                            {
                                sTypes << pColumn->dataType()->typeName();
                            }
                        }
                    }
                    if (sTypes.count() > 0)
                        rslt = m_name + "(" + sTypes.join(", ") +")";
                }

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

void Constraints::deleteConstraint(Constraint::ConstraintType type)
{
    if (m_types.testFlag(type))
    {
        for( int i = 0; i < count(); i++)
        {
            if (at(i)->type() == type)
            {
                deleteConstraint(i);
            }
        }
    }
}

/*
void Constraints::deleteConstraint(PConstraint constraint)
{
    for (int i = 0; i < count(); i++)
    {
        if (at(i).data() == constraint)
            deleteConstraint(i);
    }
}
*/
PConstraint Constraints::constraint(const Constraint::ConstraintType type) const
{
    if (m_types.testFlag(type))
    {
        for(int i = 0; i < count(); i++)
        {
            if (at(i)->type() == type)
            {
                return at(i).data();
            }
        }
    }

    return 0;
}
