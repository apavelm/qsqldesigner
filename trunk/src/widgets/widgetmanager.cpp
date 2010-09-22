#include "widgetmanager.h"
#include "../models/modelmanager.h"

WidgetManager::WidgetManager() : m_scene(0)
{
    m_tablesWidgets.clear();
}

WidgetManager::~WidgetManager()
{
    m_tablesWidgets.clear();
}

void WidgetManager::addTable(PTableModel table)
{
    if (table)
    {
        TableWidget * widget = new TableWidget(m_scene, 0, table);
        connect(widget, SIGNAL(deleteWidget(QString)), MM, SLOT(removeTable(QString)));
        m_tablesWidgets.insert(table->name(), SharedTableWidget(widget));
    }
}

void WidgetManager::removeTable(const QString& tableName)
{
    if (m_tablesWidgets.contains(tableName))
    {
        m_tablesWidgets.remove(tableName);
        // May be delete object before REMOVE?
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
