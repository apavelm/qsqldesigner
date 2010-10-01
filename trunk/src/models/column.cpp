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

#include "column.h"
#include "table.h"

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
        int i = 1;
        while (strLst.contains(newName, Qt::CaseInsensitive))
        {
            newName = defaultName + QString("_%1").arg(i++);
        }

        return newName;
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
    QString rslt = m_columnName + ": " + m_dataType.sqlTypeAcronim;
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
