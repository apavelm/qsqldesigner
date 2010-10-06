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

#ifndef COLUMN_H
#define COLUMN_H

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QPair>
#include <QtCore/QSet>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include "constraint.h"
#include "datatypes.h"

class TableModel;
typedef TableModel * PTableModel;

class ColumnModel: public QObject
{
    Q_OBJECT
public:
    ColumnModel(PTableModel table, const QString& name = QString());
    inline PTableModel table() const {return m_table;}

    inline const QString name() const {return m_columnName;}
    void setName(const QString& name);
    inline const QString comment() const {return m_columnComment;}
    void setComment(const QString& comment);
    inline PDataType dataType() const {return m_dataType;}
    void setDataType(PDataType dataType);
    inline const QPair<int, int>& dataTypeParameters() const {return m_dataTypeParameters;}
    inline void setDataTypeParameters(const QPair<int, int>& params) {m_dataTypeParameters = params;}

    inline PConstraint constraint(Constraint::ConstraintType type) const {return m_constraints.constraint(type);}
    inline const Constraints& constraints() const {return m_constraints;}
    void addConstraint(PConstraint constraint);
    inline void deleteConstraint(Constraint::ConstraintType type) {m_constraints.deleteConstraint(type);}
    inline bool isConstraintType(Constraint::ConstraintType type) const {return m_constraints.isConstraintType(type);}

    const QString getUMLColumnPrefix() const;
    const QString getUMLColumnDescription() const;
private:
    PTableModel m_table;
    QString  m_columnName;
    QString  m_columnComment;
    PDataType m_dataType;
    QPair<int, int> m_dataTypeParameters;
    Constraints m_constraints;

    const QString defaultColumnName() const;
    bool isValidName(const QString& name) const;
signals:
    void addedForeignKey();
};

typedef ColumnModel * PColumnModel;
typedef QSharedPointer<ColumnModel> SharedColumnModel;

class ColumnList: public QList<SharedColumnModel>
{
public:
    ColumnList();
    void addColumn(PColumnModel column);

    inline int getAmountForType(const Constraint::ConstraintType type) const {return m_constraintCounters[type];}
    PColumnModel getColumnByName(const QString& columnName) const;
    void getColumnsForConstraintType(const Constraint::ConstraintType type, QList<PColumnModel>& result) const;
    bool contains(const QString & columnName) const;
    void remove(const QString& columnName);
    QStringList keys() const {return m_columnNames.toList();}
private:
    int m_constraintCounters[Constraint::CT_Last];
    QSet<QString> m_columnNames;
};

#endif //COLUMN_H
