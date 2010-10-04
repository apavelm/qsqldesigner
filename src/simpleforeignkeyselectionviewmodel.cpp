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

#include "simpleforeignkeyselectionviewmodel.h"

#include "projectmanager.h"
#include "models/modelmanager.h"

SimpleForeignKeySelectionViewModel::SimpleForeignKeySelectionViewModel(QObject *parent, const DataType& datatype) : QStandardItemModel(parent)
{
    QList<QString> lstTables = CURRENTPROJECT->modelManager()->getTableList();
    foreach (const QString& tableName, lstTables)
    {
        // Create the phone groups as QStandardItems
        QStandardItem * table = new QStandardItem(QIcon(":/table24"), tableName);

        QList<QString> lstColumns =  CURRENTPROJECT->modelManager()->getColumnList(tableName, datatype);
        if (lstColumns.count() > 0)
        {
            foreach (const QString& columnName, lstColumns)
            {
                QStandardItem * column = new QStandardItem(QIcon(":/column24"), columnName);
                // the appendRow function appends the column as new row
                table->appendRow(column);
            }
            // append table as new row to the model. model takes the ownership of the item
            appendRow(table);
        }
    }
}

