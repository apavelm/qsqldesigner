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

#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include "datatypes.h"
#include "table.h"
#include "column.h"

class SqlDesignerProject;
typedef SqlDesignerProject * PSqlDesignerProject;

class ModelManager : public QObject
{
    Q_OBJECT
public:
    explicit ModelManager(QObject * parent = 0);
    inline PSqlDesignerProject project() const {return m_project;}

    PTableModel getTableByName(const QString& tableName) const;
    PColumnModel getColumnByName(const QString& tableName, const QString& columnName) const;
    inline const QList<QString> getTableList() const {return m_tables.keys();}
    const QList<QString> getColumnList(const QString& tableName, const DataType = DataType()) const;
    bool isTableNameValid(const QString& tableName) const;
    bool isConstraintNameValid(const QString& name) const;
private:
    PSqlDesignerProject m_project;
    QMap<QString, SharedTableModel> m_tables;
    QStringList m_constraintNames;
signals:
    void tableAdded(PTableModel table);
    void tableRemoved(const QString& tableName);
    void tableUpdate(const QString& tableName, PTableModel tablePtr = 0);
public slots:
    void addTable(PTableModel table);
    void removeTable(const QString& tableName);
};

typedef ModelManager * PModelManager;

#endif // MODELMANAGER_H

