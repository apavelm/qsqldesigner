#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include "../singleton.h"

#include "datatypes.h"
#include "table.h"
#include "column.h"

class ModelManager : public QObject, public Singleton<ModelManager>
{
    Q_OBJECT
public:
    PTableModel getTableByName(const QString& tableName) const;
    PColumnModel getColumnByName(const QString& tableName, const QString& columnName) const;
    inline const QList<QString> getTableList() const {return m_tables.keys();}
    const QList<QString> getTableListExceptOne(const QString& tableName) const;
    const QList<QString> getColumnList(const QString& tableName, const DataType = DataType()) const;
private:
    friend class Singleton<ModelManager>;
    ModelManager();
    virtual ~ModelManager();

    QMap<QString, SharedTableModel> m_tables;
signals:
    void tableAdded(PTableModel table);
    void tableRemoved(const QString& tableName);
    void tableUpdate(const QString& tableName, PTableModel tablePtr = 0);
public slots:
    void addTable(PTableModel table);
    void removeTable(const QString& tableName);
};

#define MM ModelManager::getInstance()

#endif // MODELMANAGER_H

