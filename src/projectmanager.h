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
    void newProject(const QString& projectName);
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
