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

#ifndef TABLECOLUMNSVIEWMODEL_H
#define TABLECOLUMNSVIEWMODEL_H

#include <QtCore/QObject>
#include <QtGui/QStandardItemModel>

#include "../models/column.h"

class TableColumnsViewModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit TableColumnsViewModel(QObject * parent);
    virtual ~TableColumnsViewModel();

    bool swapRows(int row1, int row2);
    void addColumnModel(PColumnModel column);
    void updateRow(int row, PColumnModel column);
    bool removeRow(int row);
public slots:
    void update(const ColumnList& list = ColumnList());
};

#endif // TABLECOLUMNSVIEWMODEL_H
