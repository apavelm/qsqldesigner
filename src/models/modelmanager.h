#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include "datatypes.h"
#include "table.h"
#include "column.h"

class SqlDesignerProject;
typedef SqlDesignerProject * PSqlDesignerProject;

class ModelManager : public QObject
{
    Q_OBJECT
public:
    explicit ModelManager(QObject * parent = 0);

    PTableModel getTableByName(const QString& tableName) const;
    PColumnModel getColumnByName(const QString& tableName, const QString& columnName) const;
    inline const QList<QString> getTableList() const {return m_tables.keys();}
    const QList<QString> getColumnList(const QString& tableName, const DataType = DataType()) const;
    bool isTableNameValid(const QString& tableName) const;
    bool isConstraintNameValid(const QString& name) const;
private:
    PSqlDesignerProject m_project;
    QMap<QString, SharedTableModel> m_tables;
    QStringList m_constraintNames;
signals:
    void tableAdded(PTableModel table);
    void tableRemoved(const QString& tableName);
    void tableUpdate(const QString& tableName, PTableModel tablePtr = 0);
public slots:
    void addTable(PTableModel table);
    void removeTable(const QString& tableName);
};

typedef ModelManager * PModelManager;

#endif // MODELMANAGER_H

