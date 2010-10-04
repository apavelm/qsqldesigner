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

#ifndef TABLE_H
#define TABLE_H

#include <QtGui/QApplication>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include "column.h"

class ModelManager;
typedef ModelManager * PModelManager;

class TableModel
{
    Q_DECLARE_TR_FUNCTIONS(TableModel)
public:
    TableModel(PModelManager mm = 0, const QString& name = QString());
    inline PModelManager modelManager() const {return m_mm;}

    inline const QString& name() const {return m_name;}
    void setName(const QString& name);

    inline const ColumnList& columns() const {return m_columns;}
    PColumnModel column(const QString& columnName) const;
    void setColumns(const ColumnList& newColumns);
    void addColumn(PColumnModel c);
    void removeColumn(const QString& columnName);

    const QStringList constraintsNames() const;
    inline const Constraints& tableConstraints() const {return m_constraints;}
    void addConstraint(PConstraint constraint);
    //inline void deleteConstraint(int index) {m_constraints.deleteConstraint(index);}
private:
    PModelManager m_mm;
    ColumnList m_columns;
    QString m_name;
    Constraints m_constraints;

    const QString defaultTableName() const;
};

typedef TableModel * PTableModel;
typedef QSharedPointer<TableModel> SharedTableModel;

#endif //TABLE_H
