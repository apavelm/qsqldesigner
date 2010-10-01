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

#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QtCore/QMap>
#include <QtCore/QObject>
#include "singleton.h"
#include "sqldesignerproject.h"

class ProjectManager: public QObject, public Singleton<ProjectManager>
{
    Q_OBJECT
public:
    PSqlDesignerProject currentProject();

private:
    friend class Singleton<ProjectManager>;
    ProjectManager();
    virtual ~ProjectManager();

    QMap<QString, SharedSqlDesignerProject> m_projectList;
    QString m_currentProjectName;

public slots:
    void newProject(const QString& projectName, const QString& dbmsType);
    void closeProject();
    void closeProject(const QString& projectName);
    void closeAllProjects();
    void renameProject(const QString& oldName, const QString& newName);
    void setCurrentProject(const QString& projectName);

signals:
    void currentProjectChanged(QString);
};

#define PROJECTMANAGER ProjectManager::getInstance()
#define CURRENTPROJECT ProjectManager::getInstance()->currentProject()

#endif // PROJECTMANAGER_H
