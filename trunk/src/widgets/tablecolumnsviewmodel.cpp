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

#include "tablecolumnsviewmodel.h"

TableColumnsViewModel::TableColumnsViewModel(QObject *parent) : QStandardItemModel(parent)
{
    update();
}

TableColumnsViewModel::~TableColumnsViewModel()
{
    // TODO: don't sure that it nescesary. check if Items autodeleting on ~() -> remove this code
    for (int i = rowCount() - 1; i >= 0; i--)
    {
        QList<QStandardItem *> lst = takeRow(i);
        qDeleteAll(lst);
    }
}

void TableColumnsViewModel::update(const ColumnList& list)
{
    clear();
    QStringList lstHeaderLabels;
    lstHeaderLabels << tr("PK") << tr("Name") << tr("Type") << tr("Not Null") << tr("Unique");
    setHorizontalHeaderLabels(lstHeaderLabels);

    foreach (const SharedColumnModel& column, list)
    {
        addColumnModel(column.data());
    }
}

void TableColumnsViewModel::addColumnModel(PColumnModel column)
{
    QList<QStandardItem*> rowTableLst;
    try
    {
        QStandardItem * item = new QStandardItem((column->isConstraintType(Constraint::CT_PrimaryKey) ? "PK" : "" )); // PK
        item->setTextAlignment(Qt::AlignHCenter);
        rowTableLst << item;
        item = new QStandardItem(column->name()); // Name
        item->setTextAlignment(Qt::AlignLeft);
        rowTableLst << item;
        item = new QStandardItem(column->dataType()->fullTypeName(column->dataTypeParameters())); // Type
        item->setTextAlignment(Qt::AlignHCenter);
        rowTableLst << item;
        item = new QStandardItem((column->isConstraintType(Constraint::CT_NotNull) ? "Yes" : "No" )); // Not Null
        item->setTextAlignment(Qt::AlignHCenter);
        rowTableLst << item;
        item = new QStandardItem((column->isConstraintType(Constraint::CT_Unique) ? "Yes" : "No" )); // Unique
        item->setTextAlignment(Qt::AlignHCenter);
        rowTableLst << item;
    }
    catch (...)
    {
        qDeleteAll(rowTableLst);
        rowTableLst.clear();
    }

    appendRow(rowTableLst);
}


bool TableColumnsViewModel::swapRows(int row1, int row2)
{
    if(row1 == row2)
        return false;
    if ( row1 < 0  || row2 < 0)
        return false;
    if (row1 >= rowCount() || row2 >= rowCount())
        return false;

    for (int i = 0; i < columnCount(); i++)
    {
        QString temp = item(row1, i)->text();
        item(row1, i)->setText(item(row2, i)->text());
        item(row2, i)->setText(temp);
    }
    return true;
}

void TableColumnsViewModel::updateRow(int row, PColumnModel column)
{
    item(row, 0)->setText((column->isConstraintType(Constraint::CT_PrimaryKey) ? "PK" : "" )); // PK
    item(row, 1)->setText(column->name()); // Name
    item(row, 2)->setText(column->dataType()->fullTypeName(column->dataTypeParameters())); // Type
    item(row, 3)->setText((column->isConstraintType(Constraint::CT_NotNull) ? "Yes" : "No" )); // Not Null
    item(row, 4)->setText((column->isConstraintType(Constraint::CT_Unique) ? "Yes" : "No" )); // Unique
}

bool TableColumnsViewModel::removeRow(int row)
{
    if ( row < 0   || row >= rowCount())
        return false;

    return removeRows(row, 1);
}
