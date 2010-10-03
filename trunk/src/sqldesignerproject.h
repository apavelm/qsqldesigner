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

#ifndef SQLDESIGNERPROJECT_H
#define SQLDESIGNERPROJECT_H

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtGui/QGraphicsScene>

#include <QtGui/QPrinter>
#include <QtGui/QUndoCommand>
#include <QtGui/QUndoStack>
#include <QtGui/QUndoView>

#include "models/modelmanager.h"
#include "widgets/widgetmanager.h"

#include "sqldesignerprojectsettings.h"

class SqlDesignerProject : public QObject
{
    Q_OBJECT
public:
    explicit SqlDesignerProject(const QString& projectName, const QString& dbmsType);
    ~SqlDesignerProject();

    inline const QString& name() const {return m_settings->name();}
    inline const QString& dbmsType() const {return m_settings->dbmsType();}

    inline PModelManager modelManager() {return m_modelManager.data();}
    inline PWidgetManager widgetManager() {return m_widgetManager.data();}
    inline QGraphicsScene * scene() {return m_scene.data();}
private:
    QScopedPointer<SqlDesignerProjectSettings> m_settings;
    QScopedPointer<ModelManager> m_modelManager;
    QScopedPointer<QGraphicsScene> m_scene;
    QScopedPointer<WidgetManager> m_widgetManager;
    QScopedPointer<QPrinter> m_printer;
    QScopedPointer<QUndoStack> m_undoStack;
};

typedef SqlDesignerProjectSettings * PSqlDesignerProjectSettings;
typedef SqlDesignerProject * PSqlDesignerProject;
typedef QSharedPointer<SqlDesignerProject> SharedSqlDesignerProject;

#endif // SQLDESIGNERPROJECT_H
