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

void ProjectManager::newProject(const QString& projectName)
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

    m_projectList.insert(newName, SharedSqlDesignerProject(new SqlDesignerProject(newName)));
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
