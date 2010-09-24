#ifndef WIDGETMANAGER_H
#define WIDGETMANAGER_H

#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtGui/QGraphicsScene>

#include "arrowforeignkey.h"
#include "tablewidget.h"

#include "../singleton.h"

class WidgetManager : public QObject, public Singleton<WidgetManager>
{
    Q_OBJECT
public:
    inline void setScene(QGraphicsScene * scene) {m_scene = scene;}
    PTableWidget getTableWidgetByName(const QString& tableName) const;
    PArrowForeignKey getArrowFrom(const QString& tableName, const QString& columnName) const;
    PArrowForeignKey getArrowTo(const QString& tableName, const QString& columnName) const;

    ListArrowForeignKey getArrowsFromTable(const QString& tableName) const;
    ListArrowForeignKey getArrowsToTable(const QString& tableName) const;
private:
    friend class Singleton<WidgetManager>;
    WidgetManager();
    virtual ~WidgetManager();

    QGraphicsScene * m_scene;
    QMap<QString, SharedTableWidget> m_tablesWidgets;
    QList<SharedArrowForeignKey> m_arrowsFK;

    void removeArrowBeforeTable(const QString& tableName);
public slots:
    void addTable(PTableModel table);
    void removeTable(const QString& tableName);
    void updateTable(const QString& tableName, PTableModel table = 0);
    void addArrowFK(PArrowForeignKey fk);
    void removeArrowFK(PArrowForeignKey fk);
};

#define WM WidgetManager::getInstance()

#endif // WIDGETMANAGER_H
