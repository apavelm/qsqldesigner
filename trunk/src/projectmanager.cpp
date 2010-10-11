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

    if (!m_projectList.contains(m_currentProjectName))
        return 0;

    return m_projectList.value(m_currentProjectName).data();
}

void ProjectManager::newProject(const QString& projectName, const QString& dbmsType)
{
    if (projectName.isEmpty())
        return;

    QString newName = projectName;
    QStringList names = m_projectList.keys();
    int i = 1;
    while (names.contains(newName, Qt::CaseInsensitive))
    {
        newName = projectName + QString("_%1").arg(i++);
    }

    m_projectList.insert(newName, SharedSqlDesignerProject(new SqlDesignerProject(newName, dbmsType)));
    setCurrentProject(newName);
}

void ProjectManager::closeProject()
{
    if (m_projectList.count() == 0)
        return;

    m_projectList.remove(m_currentProjectName);

    // if closing current project
    QString newName;
    if (m_projectList.count() > 0)
    {
        newName = m_projectList.keys().first();
        setCurrentProject(newName);
    }
    else
    {
        m_currentProjectName = QString();
    }
}

void ProjectManager::closeProject(const QString& projectName)
{
    if (projectName.isEmpty())
        return;

    if (!m_projectList.contains(projectName))
        return;

    m_projectList.remove(projectName);

    // if closing current project
    QString newName;
    if (m_projectList.count() > 0)
    {
        newName = m_projectList.keys().first();
        setCurrentProject(newName);
    }
    else
    {
        m_currentProjectName = QString();
    }

}

void ProjectManager::closeAllProjects()
{
    m_currentProjectName = QString();
    m_projectList.clear();
}

void ProjectManager::renameProject(const QString& oldName, const QString& newName)
{
    if (!m_projectList.contains(oldName))
        return;

    if (QString::compare(oldName, m_currentProjectName, Qt::CaseInsensitive) == 0)
        m_currentProjectName = newName;

    SharedSqlDesignerProject sp = m_projectList.value(oldName);
    m_projectList.remove(oldName);
    m_projectList.insert(newName, sp);
}

void ProjectManager::setCurrentProject(const QString& projectName)
{
    if (QString::compare(projectName, m_currentProjectName, Qt::CaseInsensitive) == 0)
        return;

    if (projectName.isEmpty())
        return;

    if (!m_projectList.contains(projectName))
        return;

    m_currentProjectName = projectName;
    emit currentProjectChanged(projectName);
}

void ProjectManager::openProject(const QString& fileName)
{
    SharedSqlDesignerProject sharedProject(SqlDesignerProject::loadProject(fileName));
    if (!sharedProject.isNull())
    {
        m_projectList.insert(sharedProject->name(), sharedProject);
        setCurrentProject(sharedProject->name());
    }
}
