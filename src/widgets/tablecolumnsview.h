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

#ifndef TABLECOLUMNSVIEW_H
#define TABLECOLUMNSVIEW_H

#include <QtGui/QTableView>
#include "tablecolumnsviewmodel.h"
#include "../models/column.h"

class TableColumnsView : public QTableView
{
    Q_OBJECT
public:
    explicit TableColumnsView(QWidget * parent = 0);
    virtual ~TableColumnsView();

    bool swapRows(int row1, int row2);
    void appendRow(PColumnModel column);
    void updateRow(int row, PColumnModel column);
    bool removeRow(int row);
    void updateColumnListModel(const ColumnList& list);
    void clear();
protected:
    void selectionChanged(const QItemSelection & selected, const QItemSelection & deselected);
private:
    TableColumnsViewModel * m_model;

    void updateHeaderSizeAndStyle();
signals:
    void selectedRowSignal(int);
};

#endif // TABLECOLUMNSVIEW_H
