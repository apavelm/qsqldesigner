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
Constraint::Constraint(PColumnModel column, const ConstraintType type, const QVariant& data) : m_table(0), m_column(column), m_type(type)
{
    m_columnName = column->name();
    m_tableName = column->table()->name();
    m_data = data;
    m_name = defaultName(type, data);
}

Constraint::Constraint(PTableModel table, const ConstraintType type, const QVariant& data) : m_table(table), m_column(0), m_type(type)
{
    m_columnName = "";
    m_tableName = table->name();
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
    bool isNameValid = false;
    if (m_column)
    {
        isNameValid = m_column->table()->modelManager()->isConstraintNameValid(newName);
    }
    else
    {
        isNameValid = m_table->modelManager()->isConstraintNameValid(newName);
    }
    if (newName.isEmpty() || isNameValid)
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
    QString rslt;
    if (type == Constraint::CT_ForeignKey)
    {
        if (var.canConvert<ConstraintForeignKey>())
        {
            ConstraintForeignKey fk = var.value<ConstraintForeignKey>();
            if (m_column)
            {
                rslt = QString("FK_%1_%2_REF_%3_%4").arg(m_tableName).arg(m_columnName).arg(fk.referenceTable()).arg(fk.referenceColumns().first());
            }
            else
            {
                rslt = QString("FK_%1_REF_%2").arg(m_tableName).arg(fk.referenceTable());
            }
        }
    }
    if (type == Constraint::CT_PrimaryKey)
    {
        rslt = QString("PK_%1").arg(m_tableName);
    }
    if (type == Constraint::CT_Unique)
    {
        if (m_column)
        {
            rslt = QString("UQ_%1_%2").arg(m_tableName).arg(m_columnName);
        }
        else
        {
            // TODO
            // table constraint for multiple columns
            // rslt =
        }
    }

    //checking for duplicate
    if (!rslt.isEmpty())
    {
        QString newName = rslt;
        int i = 1;
        bool isNameValid = false;
        while (true)
        {
            if (m_column)
            {
                isNameValid = m_column->table()->modelManager()->isConstraintNameValid(newName);
            }
            else
            {
                isNameValid = m_table->modelManager()->isConstraintNameValid(newName);
            }
            if (isNameValid)
                break;

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
                    if (m_column)
                    {
                        m_column->table()->columns().getColumnsForConstraintType(Constraint::CT_PrimaryKey, lstPrimaryKeys);
                    }
                    else
                    {
                        m_table->columns().getColumnsForConstraintType(Constraint::CT_PrimaryKey, lstPrimaryKeys);
                    }
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
                if (m_column)
                {
                    rslt = QString("%1(%2)").arg(m_name).arg(m_column->dataType()->typeName());
                }
                else
                {
                    // TODO
                    // table constraint for multiple columns
                    //rslt =
                }
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
                        const QList<QString> cols = fk.referenceColumns();

                        foreach (const QString& c, cols)
                        {
                            PModelManager mm = 0;
                            if (m_column)
                            {
                                mm = m_column->table()->modelManager();
                            }
                            else
                            {
                                mm = m_table->modelManager();
                            }

                            if (mm)
                            {
                                PTableModel table = mm->getTableByName(fk.referenceTable());
                                if (table)
                                {
                                    PColumnModel pColumn = table->column(c);
                                    if (pColumn)
                                    {
                                        sTypes << pColumn->dataType()->typeName();
                                    }
                                }
                            }
                        }
                    }

                    rslt = m_name;
                    if (sTypes.count() > 0)
                        rslt += "(" + sTypes.join(", ") +")";
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
