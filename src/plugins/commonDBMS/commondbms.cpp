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

#include "commondbms.h"

#include <QtCore/QDateTime>
#include <QtCore/QList>

#include "../../sqldesignerproject.h"

CommonDBMSPlugin::CommonDBMSPlugin() : QObject(0)
{
    m_dataTypeList << SharedDataType(new DataType("SHORTINT", 0));
    m_dataTypeList << SharedDataType(new DataType("INT", 0, "SHORTINT"));
    m_dataTypeList << SharedDataType(new DataType("BIGINT", 0, "INT;SHORTINT"));
    m_dataTypeList << SharedDataType(new DataType("CHAR", 1, "VARCHAR"));
    m_dataTypeList << SharedDataType(new DataType("VARCHAR", 1, "CHAR"));
    m_dataTypeList << SharedDataType(new DataType("DATETIME"));
    m_dataTypeList << SharedDataType(new DataType("FLOAT", 2));
}

CommonDBMSPlugin::~CommonDBMSPlugin()
{

}

QString CommonDBMSPlugin::generateDDL(PModelManager model) const
{
    QString rslt(tr("-- Generated by SQL Designer. %1\n-- Generated for project: %2\n\n").
                 arg(QDateTime::currentDateTime().toString("d MMMM yyyy, hh:mm")).
                 arg(model->projectSettings()->name()));
    if (model)
    {
        QList<QString> lstTables = model->getTableList();
        foreach (const QString& tableName, lstTables)
        {
            PTableModel pTable = model->getTableByName(tableName);
            rslt += generateDDL_Table(pTable);
        }
    }
    return rslt;
}

QString CommonDBMSPlugin::generateDDL_Table(PTableModel pTable) const
{
    QString rslt;
    if (pTable)
    {
        rslt += QString("CREATE TABLE %1 (\n").arg(pTable->name());
        // columns...
        int columnsCount = pTable->columns().count();
        for (int i = 0; i < columnsCount; i++)
        {
            PColumnModel column = pTable->columns().at(i).data();
            rslt += generateDDL_Column(column);
            i++;
            if ( (i < columnsCount) || (pTable->tableConstraints().count() > 0) )
            {
                rslt += ",";
            }

            // comment
            QString comment = column->comment().trimmed();
            if (!comment.isEmpty())
            {
                rslt += QString("  -- %1").arg(comment);
            }

            // new line
            rslt += "\n";
        }

        // table constraints print
        const Constraints& lstTableConstraint = pTable->tableConstraints();
        if (lstTableConstraint.count() > 0)
        {
            rslt += "\n";
            for (int i = 0; i < lstTableConstraint.count(); i++)
            {
                rslt += generateDDL_TableConstraint(lstTableConstraint.at(i).data());

                if (i < lstTableConstraint.count())
                {
                    rslt += ",\n";
                }
                else
                {
                    rslt += "\n";
                }
            }
        }

        rslt += ")\n\n";
    }
    return rslt;
}

QString CommonDBMSPlugin::generateDDL_TableConstraint(PConstraint pConstraint) const
{
    QString rslt;
    if (pConstraint)
    {
        rslt += QString("CONSTRAINT %1 ").arg(pConstraint->name());
        switch (pConstraint->type())
        {
            case Constraint::CT_PrimaryKey:
                {
                    rslt += "PRIMARY KEY ";
                }
                break;
            case Constraint::CT_Unique:
                {
                    rslt += "UNIQUE ";
                }
                break;
            case Constraint::CT_Check:
                {
                    rslt += "CHECK ";
                }
                break;
            case Constraint::CT_ForeignKey:
                {
                    rslt += "FOREIGN KEY ";
                }
                break;
        default: break;
        }
    }
    return rslt;
}

QString CommonDBMSPlugin::generateDDL_Column(PColumnModel pColumn) const
{
    QString rslt;
    if (pColumn)
    {
        rslt += QString("\t%1 %2").arg(pColumn->name()).arg(pColumn->dataType()->typeName());
        const Constraints& lstConstraints = pColumn->constraints();
        if (lstConstraints.isConstraintType(Constraint::CT_NotNull))
        {
            rslt += " NOT NULL";
        }
        if (lstConstraints.isConstraintType(Constraint::CT_Default))
        {
            PConstraint pConstraint = lstConstraints.constraint(Constraint::CT_Default);
            if (pConstraint)
            {
                rslt += " DEFAULT " + pConstraint->data().toString();
            }
        }
    }
    return rslt;
}

Q_EXPORT_PLUGIN2(commonDBMS, CommonDBMSPlugin);