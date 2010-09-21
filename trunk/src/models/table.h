#ifndef TABLE_H
#define TABLE_H

#include <QtGui/QApplication>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>

#include "column.h"

class TableModel
{
    Q_DECLARE_TR_FUNCTIONS(TableModel)
public:
    TableModel(const QString& name = QString());

    inline const QString& name() const {return m_name;}
    void setName(const QString& name);

    inline const ColumnList& columns() const {return m_columns;}
    void setColumns(const ColumnList& newColumns);
    void addColumn(PColumnModel c);
private:
    ColumnList m_columns;
    QString m_name;

    const QString defaultTableName() const;
    bool isValidName(const QString& name) const;
};

typedef TableModel * PTableModel;
typedef QSharedPointer<TableModel> SharedTableModel;

#endif //TABLE_H
