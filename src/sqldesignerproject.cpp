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

#include "sqldesignerproject.h"
#include "xmlhelper.h"

SqlDesignerProject::SqlDesignerProject(const QString& projectName, const QString& dbmsType) :
            QObject(0),
            m_settings(new SqlDesignerProjectSettings(projectName, dbmsType)),
            m_modelManager(new ModelManager(this, m_settings.data())),
            m_scene(new QGraphicsScene(this)),
            m_widgetManager(new WidgetManager(this, m_scene.data())),
            m_printer(new QPrinter(QPrinter::HighResolution)),
            m_undoStack(new QUndoStack(this))
{
    createConnections();
}

SqlDesignerProject::SqlDesignerProject(PSqlDesignerProjectSettings settings, PModelManager mm, const QList<QPair<QString, QPointF> >& coords) :
            QObject(0),
            m_settings(settings),
            m_modelManager(mm),
            m_scene(new QGraphicsScene(this)),
            m_widgetManager(new WidgetManager(this, m_scene.data())),
            m_printer(new QPrinter(QPrinter::HighResolution)),
            m_undoStack(new QUndoStack(this))
{
    m_widgetManager->getWidgetsFromModelManager(mm, coords);
    createConnections();
}

void SqlDesignerProject::createConnections()
{
    connect(m_modelManager.data(), SIGNAL(tableAdded(PTableModel)), m_widgetManager.data(), SLOT(addTable(PTableModel)) );
    connect(m_modelManager.data(), SIGNAL(tableRemoved(QString)), m_widgetManager.data(), SLOT(removeTable(QString)) );
    connect(m_modelManager.data(), SIGNAL(tableUpdated(QString,PTableModel)), m_widgetManager.data(), SLOT(updateTable(QString,PTableModel)) );

    connect(m_modelManager.data(), SIGNAL(tableAdded(PTableModel)), SIGNAL(modelChanged()));
    connect(m_modelManager.data(), SIGNAL(tableRemoved(QString)), SIGNAL(modelChanged()));
    connect(m_modelManager.data(), SIGNAL(tableUpdated(QString,PTableModel)), SIGNAL(modelChanged()));
}

SqlDesignerProject::~SqlDesignerProject()
{
}

bool SqlDesignerProject::saveProject(const QString fileName)
{
    return XmlHelper::save(fileName, this);
}

PSqlDesignerProject SqlDesignerProject::loadProject(const QString fileName)
{
    return XmlHelper::read(fileName);
}
