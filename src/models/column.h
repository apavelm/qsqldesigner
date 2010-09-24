#ifndef COLUMN_H
#define COLUMN_H

#include <QtGui/QApplication>
#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

#include "constraint.h"
#include "datatypes.h"

class TableModel;
typedef TableModel * PTableModel;

class ColumnModel
{
    Q_DECLARE_TR_FUNCTIONS(ColumnModel)
public:
    ColumnModel(PTableModel table, const QString& name = QString());
    inline PTableModel table() const {return m_parent;}

    inline const QString name() const {return m_columnName;}
    void setName(const QString& name);
    inline const QString comment() const {return m_columnComment;}
    void setComment(const QString& comment);
    inline const DataType& dataType() const {return m_dataType;}
    void setDataType(const DataType& dataType);

    inline const Constraints& constraints() const {return m_constraints;}
    inline void addConstraint(PConstraint constraint) {m_constraints.addConstraint(constraint);}
    inline void deleteConstraint(int index) {m_constraints.deleteConstraint(index);}
    inline void deleteConstraint(PConstraint constraint) {m_constraints.deleteConstraint(constraint);}
    inline bool isConstraintType(Constraint::ConstraintType type) const {return m_constraints.isConstraintType(type);}

    const QString getUMLColumnPrefix() const;
    const QString getUMLColumnDescription() const;
private:
    PTableModel m_parent;
    QString  m_columnName;
    QString  m_columnComment;
    DataType m_dataType;
    Constraints m_constraints;

    const QString defaultColumnName() const;
    bool isValidName(const QString& name) const;
};

typedef ColumnModel * PColumnModel;
typedef QSharedPointer<ColumnModel> SharedColumnModel;

class ColumnList: public QMap<QString, SharedColumnModel>
{
public:
    ColumnList();
    void addColumn(PColumnModel column);

    inline int getAmountForType(const Constraint::ConstraintType type) const {return m_constraintCounters[type];}
    PColumnModel getColumnByName(const QString& columnName) const;
    void getColumnsForConstraintType(const Constraint::ConstraintType type, QList<PColumnModel>& result) const;
private:
    int m_constraintCounters[Constraint::CT_Last];
};

#endif //COLUMN_H
