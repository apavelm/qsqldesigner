#ifndef WIDGETMANAGER_H
#define WIDGETMANAGER_H

#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtGui/QGraphicsScene>

#include "arrowforeignkey.h"
#include "tablewidget.h"

class SqlDesignerProject;
typedef SqlDesignerProject * PSqlDesignerProject;

class WidgetManager : public QObject
{
    Q_OBJECT
public:
    explicit WidgetManager(QObject * parent = 0, QGraphicsScene * scene = 0);
    ~WidgetManager();

    PTableWidget getTableWidgetByName(const QString& tableName) const;

private:
    PSqlDesignerProject m_project;
    QGraphicsScene * m_scene;
    QMap<QString, SharedTableWidget> m_tablesWidgets;
    QList<SharedArrowForeignKey> m_arrowsFK;

    void removeArrowBeforeTable(const QString& tableName);
    //PArrowForeignKey getArrowFrom(const QString& tableName, const QString& columnName) const;
    //PArrowForeignKey getArrowTo(const QString& tableName, const QString& columnName) const;
    ListArrowForeignKey getArrowsFromTable(const QString& tableName) const;
    ListArrowForeignKey getArrowsToTable(const QString& tableName) const;
public slots:
    void addTable(PTableModel table);
    void removeTable(const QString& tableName);
    void updateTable(const QString& tableName, PTableModel table = 0);
    void addArrowFK(PConstraint constraint);
    //void removeArrowFK(PConstraint constraint);
};

typedef WidgetManager * PWidgetManager;

#endif // WIDGETMANAGER_H
