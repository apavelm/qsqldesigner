#ifndef TABLE_H
#define TABLE_H

#include <QtGui/QApplication>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include "column.h"

class ModelManager;
typedef ModelManager * PModelManager;

class TableModel
{
    Q_DECLARE_TR_FUNCTIONS(TableModel)
public:
    TableModel(PModelManager mm = 0, const QString& name = QString());
    inline PModelManager modelManager() const {return m_mm;}

    inline const QString& name() const {return m_name;}
    void setName(const QString& name);

    inline const ColumnList& columns() const {return m_columns;}
    void setColumns(const ColumnList& newColumns);
    void addColumn(PColumnModel c);
    void removeColumn(const QString& columnName);

    const QStringList constraintsNames() const;
    inline const Constraints& tableConstraints() const {return m_constraints;}
    void addConstraint(PConstraint constraint);
    inline void deleteConstraint(int index) {m_constraints.deleteConstraint(index);}
private:
    PModelManager m_mm;
    ColumnList m_columns;
    QString m_name;
    Constraints m_constraints;

    const QString defaultTableName() const;
};

typedef TableModel * PTableModel;
typedef QSharedPointer<TableModel> SharedTableModel;

#endif //TABLE_H
