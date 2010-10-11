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

#include "widgetmanager.h"
#include "../sqldesignerproject.h"
#include "../models/modelmanager.h"

const qreal TABLE_Z_ORDER = 30.0;

WidgetManager::WidgetManager(QObject * parent, QGraphicsScene * scene) : QObject(parent), m_scene(scene)
{
    m_project = qobject_cast<PSqlDesignerProject>(parent);
    m_tablesWidgets.clear();
    m_arrowsFK.clear();
}

WidgetManager::~WidgetManager()
{
    m_scene = 0;
    m_arrowsFK.clear();
    m_tablesWidgets.clear();
}

void WidgetManager::addTable(PTableModel table)
{
    if (table)
    {
        PTableWidget widget = new TableWidget(m_scene, 0, table);
        widget->setZValue(TABLE_Z_ORDER);
        connect(widget, SIGNAL(deleteWidget(QString)), m_project->modelManager(), SLOT(removeTable(QString)));
        connect(widget, SIGNAL(editWidget(QString)), m_project, SIGNAL(editTable(QString)));
        m_tablesWidgets.insert(table->name(), SharedTableWidget(widget));

        // check for FK constraints
        if (table->hasForeignKeys())
        {
            foreach (PConstraint cn, table->foreignKeys())
            {
                //add graphics arrow
                addArrowFK(cn);
            }
        }
    }
}

void WidgetManager::removeTable(const QString& tableName)
{
    if (m_tablesWidgets.contains(tableName))
    {
        removeArrowBeforeTable(tableName);
        m_tablesWidgets.remove(tableName);
    }
}

void WidgetManager::updateTable(const QString& tableName, PTableModel table)
{
    if (table)
    {
        QMap<QString,SharedTableWidget>::iterator i = m_tablesWidgets.find(tableName);
        if (i != m_tablesWidgets.end())
        {
            SharedTableWidget widget = i.value();
            widget->setModel(table);
            if (tableName != widget->name())
            {
                m_tablesWidgets.remove(tableName);
                m_tablesWidgets.insert(widget->name(), widget);
            }
            widget->update();
        }
    }
    else
    {
        QMap<QString,SharedTableWidget>::const_iterator i = m_tablesWidgets.constFind(tableName);
        if (i != m_tablesWidgets.constEnd())
            i.value()->update();
    }

}

void WidgetManager::removeArrowBeforeTable(const QString& tableName)
{
    for(QList<SharedArrowForeignKey>::iterator arrow = m_arrowsFK.begin(); arrow != m_arrowsFK.end(); ++arrow)
    {
        if (QString::compare((*arrow)->sourceTable()->name(), tableName, Qt::CaseInsensitive) == 0 ||
            QString::compare((*arrow)->refTable()->name(), tableName, Qt::CaseInsensitive) == 0)
        {
            m_arrowsFK.erase(arrow);
        }
    }
}

PTableWidget WidgetManager::getTableWidgetByName(const QString& tableName) const
{
    if (tableName.isEmpty())
        return 0;
    if (!m_tablesWidgets.contains(tableName))
        return 0;

    QMap<QString, SharedTableWidget>::const_iterator i = m_tablesWidgets.constFind(tableName);
    if (i != m_tablesWidgets.constEnd())
    {
        return i.value().data();
    }
    else
    {
        return 0;
    }
}

ListArrowForeignKey WidgetManager::getArrowsFromTable(const QString& tableName) const
{
    ListArrowForeignKey lst;
    foreach (const SharedArrowForeignKey& arrow, m_arrowsFK)
    {
        if (QString::compare(arrow->sourceTable()->name(), tableName, Qt::CaseInsensitive) == 0)
        {
            lst << arrow.data();
        }
    }
    return lst;
}

ListArrowForeignKey WidgetManager::getArrowsToTable(const QString& tableName) const
{
    ListArrowForeignKey lst;
    foreach (const SharedArrowForeignKey& arrow, m_arrowsFK)
    {
        if (QString::compare(arrow->refTable()->name(), tableName, Qt::CaseInsensitive) == 0)
        {
            lst << arrow.data();
        }
    }
    return lst;
}

void WidgetManager::addArrowFK(PConstraint constraint)
{
    if (constraint)
    {
        if (constraint->type() == Constraint::CT_ForeignKey)
        {
            PArrowForeignKey arrow = new ArrowForeignKey(this, constraint);
            m_arrowsFK.append(SharedArrowForeignKey(arrow));
            m_scene->addItem(arrow);
        }
    }
}
/*
void WidgetManager::removeArrowFK(PConstraint constraint)
{
    if (constraint)
    {
        QVariant var = constraint->data();
        if (var.canConvert<ConstraintForeignKey>())
        {
            ConstraintForeignKey fk = var.value<ConstraintForeignKey>();
            QList<SharedArrowForeignKey>::iterator arrow;
            for(arrow = m_arrowsFK.begin(); arrow != m_arrowsFK.end(); ++arrow)
            {
                if (QString::compare((*arrow)->refTable()->name(), fk.referenceTable(), Qt::CaseInsensitive) == 0 &&
                    QString::compare((*arrow)->sourceTable()->name(), constraint->column()->table()->name(), Qt::CaseInsensitive) == 0 &&
                    QString::compare((*arrow)->sourceColumn()->name(), constraint->column()->name(), Qt::CaseInsensitive) == 0 &&
                    QString::compare((*arrow)->refColumn()->name(), fk.referenceColumns().first(), Qt::CaseInsensitive) == 0)
                {
                    m_arrowsFK.erase(arrow);
                }
            }
        }
    }
}
*/

void WidgetManager::addedSimpleForeignKey(QString tableName, QString columnName)
{

}

void WidgetManager::removedSimpleForeignKey(QString tableName, QString columnName)
{

}

void WidgetManager::addedForeignKey(QString tableName, QString constraintName)
{

}

void WidgetManager::removedForeignKey(QString tableName, QString constraintName)
{

}
