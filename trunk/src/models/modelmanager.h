#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QtCore/QObject>

#include "table.h"

class ModelManager : public QObject
{
    Q_OBJECT
public:
    ModelManager(QObject * parent = 0);
    ~ModelManager();

    void addTable();
    void removeTable();
private:
    QList<TableModel> m_tables;
};

#endif // MODELMANAGER_H

