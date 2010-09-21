#ifndef WIDGETMANAGER_H
#define WIDGETMANAGER_H

#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtGui/QGraphicsScene>

#include "tablewidget.h"

#include "../singleton.h"

class WidgetManager : public QObject, public Singleton<WidgetManager>
{
    Q_OBJECT
public:
    inline void setScene(QGraphicsScene * scene) {m_scene = scene;}
private:
    friend class Singleton<WidgetManager>;
    WidgetManager();
    virtual ~WidgetManager();

    QGraphicsScene * m_scene;
    QMap<QString, SharedTableWidget> m_tablesWidgets;
public slots:
    void addTable(PTableModel table);
    void removeTable(const QString& tableName);
    void updateTable(const QString& tableName, PTableModel table = 0);
};

#define WM WidgetManager::getInstance()

#endif // WIDGETMANAGER_H
