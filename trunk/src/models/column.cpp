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
ColumnModel::ColumnModel(PTableModel table, const QString& name) : QObject(table), m_table(table), m_dataType(0)
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


void ColumnModel::addConstraint(PConstraint constraint)
{
    if (constraint)
    {
        m_constraints.addConstraint(constraint);
        if (constraint->type() == Constraint::CT_ForeignKey)
        {
            emit addedForeignKey();
        }
    }
}

void ColumnModel::setComment(const QString& comment)
{
    m_columnComment = comment;
}

void ColumnModel::setDataType(PDataType dataType)
{
    m_dataType = dataType;
    m_dataTypeParameters.first = 0;
    m_dataTypeParameters.second = 0;
}

const QString ColumnModel::defaultColumnName() const
{
    if (m_table)
    {
        QStringList strLst(m_table->columns().keys());
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
    if (m_table)
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
    QString rslt = m_columnName + ": " + m_dataType->typeName();
    if (m_dataType->parametersAmount() > 0)
    {
        rslt += "(" + QString::number(m_dataTypeParameters.first);
        if (m_dataType->parametersAmount() > 1)
        {
            rslt += "," + QString::number(m_dataTypeParameters.second);
        }
        rslt += ")";
    }
    return rslt;
}

// Column List

ColumnList::ColumnList() : QList<SharedColumnModel>()
{
    qFill(&m_constraintCounters[0], &m_constraintCounters[Constraint::CT_Last], 0 );
}

void ColumnList::addColumn(PColumnModel column)
{
    if (column)
    {
        if (!m_columnNames.contains(column->name()))
        {
            foreach (const SharedConstraint& c, column->constraints())
            {
                m_constraintCounters[c->type()]++;
            }
            append(SharedColumnModel(column));
            m_columnNames.insert(column->name());
        }
        else
        {
            // throw exception
        }
    }
}

PColumnModel ColumnList::getColumnByName(const QString& columnName) const
{
    for (ColumnList::const_iterator i = this->constBegin(); i != this->constEnd(); ++i)
    {
        if (QString::compare( (*i)->name(), columnName, Qt::CaseInsensitive) == 0)
        {
           return i->data();
        }
    }
    return 0;
}

void ColumnList::getColumnsForConstraintType(const Constraint::ConstraintType type, QList<PColumnModel>& result) const
{
    result.clear();
    if (m_constraintCounters[type] > 0)
    {
        for (ColumnList::const_iterator i = this->constBegin(); i != this->constEnd(); ++i)
        {
            if ((*i)->isConstraintType(type))
            {
                result.append((*i).data());
            }
        }
    }
}

bool ColumnList::contains(const QString & columnName) const
{
    return m_columnNames.contains(columnName);
}

void ColumnList::remove(const QString& columnName)
{
    for (ColumnList::iterator i = this->begin(); i != this->end(); ++i)
    {
        if (QString::compare( (*i)->name(), columnName, Qt::CaseInsensitive) == 0)
        {
            erase(i);
            m_columnNames.remove(columnName);
            if (i == this->end())
                break;
        }
    }
}

void ColumnList::remove(int index)
{
    if (index < 0 || index >= count())
        return;
    QString name = at(index)->name();
    remove(name);
}
