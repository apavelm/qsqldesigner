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

#include "tablecolumnsview.h"
#include <QtGui/QHeaderView>

TableColumnsView::TableColumnsView(QWidget * parent) : QTableView(parent)
{
    m_model = new TableColumnsViewModel(this);
    setModel(m_model);
}

TableColumnsView::~TableColumnsView()
{
    if (m_model)
        delete m_model;
}

bool TableColumnsView::swapRows(int row1, int row2)
{
    const QModelIndexList selLst = selectedIndexes();
    bool rslt = m_model->swapRows(row1, row2);
    if (rslt)
    {
        if (selLst.count() > 0)
        {
            selectRow((selLst.at(0).row() == row1 ? row2 : row1));
        }
    }
    return rslt;
}

void TableColumnsView::updateRow(int row, PColumnModel column)
{
    m_model->updateRow(row, column);
}

void TableColumnsView::updateColumnListModel(const ColumnList& list)
{
    m_model->update(list);
    updateHeaderSizeAndStyle();
}

void TableColumnsView::updateHeaderSizeAndStyle()
{
    horizontalHeader()->setHighlightSections(false);
    horizontalHeader()->setClickable(false);
    horizontalHeader()->resizeSection(0, 35);
    horizontalHeader()->resizeSection(1, 200);
    horizontalHeader()->resizeSection(2, 100);
    horizontalHeader()->resizeSection(3, 50);
    horizontalHeader()->resizeSection(4, 50);
}

void TableColumnsView::clear()
{
    m_model->update();
    updateHeaderSizeAndStyle();
}

void TableColumnsView::appendRow(PColumnModel column)
{
    m_model->addColumnModel(column);
}

bool TableColumnsView::removeRow(int row)
{
    return m_model->removeRow(row);
}

void TableColumnsView::selectionChanged(const QItemSelection & selected, const QItemSelection & deselected)
{
    QModelIndexList selLst = selected.indexes();
    QModelIndexList deselLst = deselected.indexes();

    if (selected.count() == 0)
    {
        emit selectedRowSignal(-1);
    }
    else
    {
        emit selectedRowSignal(selLst.at(0).row());
    }
    QTableView::selectionChanged(selected, deselected);
}
