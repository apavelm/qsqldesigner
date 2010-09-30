#include "sqldesignerproject.h"

SqlDesignerProject::SqlDesignerProject(const QString& projectName) :
            QObject(0),
            m_modelManager(new ModelManager(this)),
            m_scene(new QGraphicsScene(this)),
            m_widgetManager(new WidgetManager(this, m_scene.data())),
            m_projectName(projectName)
{
    m_scene->setSceneRect(0, 0, 600, 400);
    connect(m_modelManager.data(), SIGNAL(tableAdded(PTableModel)), m_widgetManager.data(), SLOT(addTable(PTableModel)) );
    connect(m_modelManager.data(), SIGNAL(tableRemoved(QString)), m_widgetManager.data(), SLOT(removeTable(QString)) );
    connect(m_modelManager.data(), SIGNAL(tableUpdate(QString,PTableModel)), m_widgetManager.data(), SLOT(updateTable(QString,PTableModel)) );
}

void SqlDesignerProject::rename(const QString& newName)
{
    if (newName.isEmpty())
        return;
    m_projectName = newName;
}
