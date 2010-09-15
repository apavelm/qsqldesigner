#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include "../singleton.h"

#include "table.h"

class ModelManager : public QObject, public Singleton<ModelManager>
{
    Q_OBJECT
public:
    void addTable(PTableModel table);
    void removeTable(const QString& tableName);
    PTableModel getTableByName(const QString& tableName);
    QString getNewDefaultNameForTable() const;
private:
    friend class Singleton<ModelManager>;
    ModelManager();
    virtual ~ModelManager();

    QMap<QString, SharedTableModel> m_tables;
signals:
    void tableAdded(PTableModel table);
    void tableRemoved(const QString& tableName);
    void tableUpdate(const QString& tableName);
};

#define MM ModelManager::getInstance()

#endif // MODELMANAGER_H

