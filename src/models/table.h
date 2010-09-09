#ifndef TABLE_H
#define TABLE_H

#include <QtCore/QList>
#include <QtCore/QString>

#include "column.h"

class TableModel
{
public:
    TableModel(const QString& name);

    inline const QString& name() const {return m_name;}
    void setName(const QString& name);

    inline QList<ColumnModel> columns() const {return m_columns;}
    void setColumns(const QList<ColumnModel>& newColumns);
    void addColumn(const ColumnModel c);

private:
    QList<ColumnModel> m_columns;
    QString m_name;
};

#endif //TABLE_H
