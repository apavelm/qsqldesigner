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

#include "table.h"

#include "modelmanager.h"

TableModel::TableModel(PModelManager mm, const QString& name) : QObject(mm)
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

PColumnModel TableModel::column(const QString& columnName) const
{
    return m_columns.getColumnByName(columnName);
}

PColumnModel TableModel::column(int index) const
{
    if (index < 0 || index >= m_columns.count())
        return 0;
    return m_columns.at(index).data();
}

void TableModel::addColumn(PColumnModel c)
{
    m_columns.addColumn(c);
    if (c->isConstraintType(Constraint::CT_ForeignKey))
    {
        emit addedSimpleForeignKey(m_name, c->name());
    }
}

void TableModel::removeColumn(const QString& columnName)
{
    PColumnModel col = m_columns.getColumnByName(columnName);
    if (col)
    {
        if (col->isConstraintType(Constraint::CT_ForeignKey))
        {
            emit removedSimpleForeignKey(m_name, columnName);
        }
        m_columns.remove(columnName);
    }
}

bool TableModel::removeColumn(int index)
{
    if (index < 0 || index >= m_columns.count())
        return false;

    PColumnModel col = m_columns.at(index).data();
    if (col)
    {
        if (col->isConstraintType(Constraint::CT_ForeignKey))
        {
            emit removedSimpleForeignKey(m_name, col->name());
        }
        m_columns.remove(index);
        return true;
    }
    return false;
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

void TableModel::swapColumns(int row1, int row2)
{
    if (row1 == row2)
        return;

    if (row1 < 0 || row2 < 0)
        return;

    if (row1 >= m_columns.count() || row2 >= m_columns.count())
            return;

    m_columns.swap(row1, row2);
}

bool TableModel::hasForeignKeys() const
{
    foreach(const SharedConstraint& cn, m_constraints)
    {
        if (cn->type() == Constraint::CT_ForeignKey)
        {
            return true;
        }
    }
    foreach(const SharedColumnModel& c, m_columns)
    {
        if (c->isConstraintType(Constraint::CT_ForeignKey))
        {
            return true;
        }
    }
    return false;
}

QList<PConstraint> TableModel::foreignKeys() const
{
    QList<PConstraint> rslt;

    foreach(const SharedConstraint& cn, m_constraints)
    {
        if (cn->type() == Constraint::CT_ForeignKey)
        {
            rslt << cn.data();
        }
    }
    foreach(const SharedColumnModel& c, m_columns)
    {
        if (c->isConstraintType(Constraint::CT_ForeignKey))
        {
            rslt << c->constraint(Constraint::CT_ForeignKey);
        }
    }

    return rslt;
}
