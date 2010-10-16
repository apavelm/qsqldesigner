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

#include "projectmanager.h"

ProjectManager::ProjectManager()
{
}

ProjectManager::~ProjectManager()
{
}

PSqlDesignerProject ProjectManager::currentProject()
{
    if (m_currentProjectName.isEmpty())
        return 0;

    if (m_projectList.count() == 0)
        return 0;

    QList<SharedSqlDesignerProject>::const_iterator i;
    for (i = m_projectList.constBegin(); i != m_projectList.constEnd(); ++i)
    {
        if ((*i)->name() ==  m_currentProjectName)
        {
            break;
        }
    }
    if (i == m_projectList.constEnd())
        return 0;

    return i->data();
}

QList<QString> ProjectManager::projectNames() const
{
    QList<QString> rslt;
    QList<SharedSqlDesignerProject>::const_iterator i;
    for (i = m_projectList.constBegin(); i != m_projectList.constEnd(); ++i)
    {
        rslt << (*i)->name();
    }
    return rslt;
}

void ProjectManager::newProject(const QString& projectName, const QString& dbmsType)
{
    if (projectName.isEmpty())
        return;
    QString newName = defaultProjectName(projectName);
    m_projectList.append(SharedSqlDesignerProject(new SqlDesignerProject(newName, dbmsType)));
    setCurrentProject(newName);
}

void ProjectManager::closeProject(const QString& projectName)
{
    QString projToClose = (projectName.isEmpty() ? m_currentProjectName : projectName);
    if (projToClose.isEmpty())
        return;

    QStringList names(projectNames());
    if (!names.contains(projToClose, Qt::CaseInsensitive))
        return;

    QList<SharedSqlDesignerProject>::iterator i;
    for (i = m_projectList.begin(); i != m_projectList.end(); ++i)
    {
        if (QString::compare((*i)->name(), projToClose, Qt::CaseInsensitive) == 0)
        {
            m_projectList.erase(i);
            break;
        }
    }

    // if closing current project
    QString newName;
    if (m_projectList.count() > 0)
    {
        newName = names.first();
        setCurrentProject(newName);
    }
    else
    {
        m_currentProjectName = QString();
        emit currentProjectChanged(QString());
    }
}

void ProjectManager::closeAllProjects()
{
    m_currentProjectName = QString();
    m_projectList.clear();
    emit currentProjectChanged(QString());
}

void ProjectManager::setCurrentProject(const QString& projectName)
{
    if (QString::compare(projectName, m_currentProjectName, Qt::CaseInsensitive) == 0)
        return;

    if (projectName.isEmpty())
        return;

    QStringList names = projectNames();
    if (!names.contains(projectName, Qt::CaseInsensitive))
        return;

    m_currentProjectName = projectName;
    emit currentProjectChanged(projectName);
}

void ProjectManager::openProject(const QString& fileName)
{
    PSqlDesignerProject project = SqlDesignerProject::loadProject(fileName);
    if (project)
    {
        m_projectList.append(SharedSqlDesignerProject(project));
        setCurrentProject(project->name());
    }
}

QString ProjectManager::defaultProjectName(const QString projectName)
{
    QString newName = projectName;
    QStringList names(projectNames());
    int i = 1;
    while (names.contains(newName, Qt::CaseInsensitive))
    {
        newName = projectName + QString("_%1").arg(i++);
    }
    return newName;
}
