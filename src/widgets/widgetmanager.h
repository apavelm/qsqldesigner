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

#ifndef WIDGETMANAGER_H
#define WIDGETMANAGER_H

#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtGui/QGraphicsScene>

#include "arrowforeignkey.h"
#include "tablewidget.h"

class SqlDesignerProject;
typedef SqlDesignerProject * PSqlDesignerProject;
class ModelManager;
typedef ModelManager * PModelManager;

class WidgetManager : public QObject
{
    Q_OBJECT
public:
    explicit WidgetManager(QObject * parent = 0, QGraphicsScene * scene = 0);
    ~WidgetManager();

    PTableWidget getTableWidgetByName(const QString& tableName) const;
    QMap<QString, SharedTableWidget> tablesWidgets() const {return m_tablesWidgets;}

    void getWidgetsFromModelManager(PModelManager mm, const QList<QPair<QString, QPointF> >& coords);
private:
    PSqlDesignerProject m_project;
    QGraphicsScene * m_scene;
    QMap<QString, SharedTableWidget> m_tablesWidgets;
    QList<SharedArrowForeignKey> m_arrowsFK;

    void removeArrowBeforeTable(const QString& tableName);
    ListArrowForeignKey getArrowsFromTable(const QString& tableName) const;
    ListArrowForeignKey getArrowsToTable(const QString& tableName) const;
private slots:
    void addedSimpleForeignKey(QString tableName, QString columnName);
    void removedSimpleForeignKey(QString tableName, QString columnName);
    void addedForeignKey(QString tableName, QString constraintName);
    void removedForeignKey(QString tableName, QString constraintName);
public slots:
    void addTable(PTableModel table, QPointF pos = QPointF());
    void removeTable(const QString& tableName);
    void updateTable(const QString& tableName, PTableModel table = 0);
    void addArrowFK(PConstraint constraint);
    //void removeArrowFK(PConstraint constraint);
};

typedef WidgetManager * PWidgetManager;

#endif // WIDGETMANAGER_H
