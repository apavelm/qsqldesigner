#ifndef SQLDESIGNERPROJECT_H
#define SQLDESIGNERPROJECT_H

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtGui/QGraphicsScene>

#include "models/modelmanager.h"
#include "widgets/widgetmanager.h"


class SqlDesignerProject : public QObject
{
    Q_OBJECT
public:
    explicit SqlDesignerProject(const QString& projectName);
    inline PModelManager modelManager() {return m_modelManager.data();}
    inline PWidgetManager widgetManager() {return m_widgetManager.data();}
    inline QGraphicsScene * scene() {return m_scene.data();}
    void rename(const QString& newName);
private:
    QScopedPointer<ModelManager> m_modelManager;
    QScopedPointer<QGraphicsScene> m_scene;
    QScopedPointer<WidgetManager> m_widgetManager;
    QString m_projectName;
};

typedef SqlDesignerProject * PSqlDesignerProject;
typedef QSharedPointer<SqlDesignerProject> SharedSqlDesignerProject;

#endif // SQLDESIGNERPROJECT_H
