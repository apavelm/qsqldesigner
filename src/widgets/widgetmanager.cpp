#include "widgetmanager.h"
#include "../models/modelmanager.h"

WidgetManager::WidgetManager() : m_scene(0)
{
    m_tablesWidgets.clear();
    m_arrowsFK.clear();
}

WidgetManager::~WidgetManager()
{
    m_arrowsFK.clear();
    m_tablesWidgets.clear();
}

void WidgetManager::addTable(PTableModel table)
{
    if (table)
    {
        PTableWidget widget = new TableWidget(m_scene, 0, table);
        connect(widget, SIGNAL(deleteWidget(QString)), MM, SLOT(removeTable(QString)));
        m_tablesWidgets.insert(table->name(), SharedTableWidget(widget));

        // check for FK constraints
        if (table->isConstraintType(Constraint::CT_ForeignKey))
        {
            // for each FK adding ArrowForeignKeyWidget
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

PArrowForeignKey WidgetManager::getArrowFrom(const QString& tableName, const QString& columnName) const
{
    foreach (const SharedArrowForeignKey& arrow, m_arrowsFK)
    {
        if (arrow->isValid())
        {
            if (QString::compare(arrow->sourceTable()->name(), tableName, Qt::CaseInsensitive) == 0 &&
                QString::compare(arrow->sourceColumn()->name(), columnName, Qt::CaseInsensitive) == 0)
            {
                return arrow.data();
            }
        }
    }
    return 0;
}

PArrowForeignKey WidgetManager::getArrowTo(const QString& tableName, const QString& columnName) const
{
    foreach (const SharedArrowForeignKey& arrow, m_arrowsFK)
    {
        if (arrow->isValid())
        {
            if (QString::compare(arrow->refTable()->name(), tableName, Qt::CaseInsensitive) == 0 &&
                QString::compare(arrow->refColumn()->name(), columnName, Qt::CaseInsensitive) == 0)
            {
                return arrow.data();
            }
        }
    }
    return 0;
}

ListArrowForeignKey WidgetManager::getArrowsFromTable(const QString& tableName) const
{
    ListArrowForeignKey lst;
    foreach (const SharedArrowForeignKey& arrow, m_arrowsFK)
    {
        if (arrow->isValid())
        {
            if (QString::compare(arrow->sourceTable()->name(), tableName, Qt::CaseInsensitive) == 0)
            {
                lst << arrow.data();
            }
        }
    }
    return lst;
}

ListArrowForeignKey WidgetManager::getArrowsToTable(const QString& tableName) const
{
    ListArrowForeignKey lst;
    foreach (const SharedArrowForeignKey& arrow, m_arrowsFK)
    {
        if (arrow->isValid())
        {
            if (QString::compare(arrow->refTable()->name(), tableName, Qt::CaseInsensitive) == 0)
            {
                lst << arrow.data();
            }
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
            PArrowForeignKey arrow = new ArrowForeignKey(constraint);
            m_arrowsFK.append(SharedArrowForeignKey(arrow));
            m_scene->addItem(arrow);
        }
    }
}

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
                    QString::compare((*arrow)->refColumn()->name(), fk.referenceColumn(), Qt::CaseInsensitive) == 0)
                {
                    m_arrowsFK.erase(arrow);
                }
            }
        }
    }
}
