#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include "../singleton.h"

#include "table.h"

class ModelManager : public Singleton<ModelManager>
{
public:
    void addTable();
    void removeTable();
private:
    friend class Singleton<ModelManager>;
    ModelManager();
    virtual ~ModelManager();


    QList<TableModel> m_tables;
};

#define MM ModelManager::getInstance()

#endif // MODELMANAGER_H

