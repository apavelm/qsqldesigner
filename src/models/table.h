#ifndef TABLE_H
#define TABLE_H

#include <QtCore/QList>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtGui/QFontMetrics>

#include "column.h"

class TableModel
{
public:
    TableModel(const QString& name);

    inline const QString& name() const {return m_name;}
    void setName(const QString& name);

    inline const ColumnList& columns() const {return m_columns;}
    void setColumns(const ColumnList& newColumns);
    void addColumn(PColumnModel c);

    qreal longestStringWidth(const QFontMetrics& metrics) const;
private:
    ColumnList m_columns;
    QString m_name;
};

typedef TableModel * PTableModel;
typedef QSharedPointer<TableModel> SharedTableModel;

#endif //TABLE_H
