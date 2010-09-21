#ifndef COLUMN_H
#define COLUMN_H

#include <QtGui/QApplication>
#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

#include "datatypes.h"

class TableModel;
typedef TableModel * PTableModel;

class ColumnConstraintForeignKey
{
public:
    enum Type {ForeignKeyType = QVariant::UserType + 1};

    ColumnConstraintForeignKey(const QString& tableSource = QString(), const QString& columnSource = QString(), const QString& tableRef = QString(), const QString& columnRef = QString())
    {
        m_source = qMakePair(tableSource, columnSource);
        m_reference = qMakePair(tableRef, columnRef);
    }

    ColumnConstraintForeignKey(const ColumnConstraintForeignKey& old)
    {
        if (old.isValid())
        {
            m_source = old.source();
            m_reference = old.reference();
        }
    }

    inline const QPair<QString, QString> source() const {return m_source;}
    inline const QString sourceTable() const {return m_source.first;}
    inline const QString sourceColumn() const {return m_source.second;}
    inline const QPair<QString, QString> reference() const {return m_reference;}
    inline const QString referenceTable() const {return m_reference.first;}
    inline const QString referenceColumn() const {return m_reference.second;}

    bool isValid() const
    {
        return !m_source.first.isEmpty() && !m_source.second.isEmpty() && !m_reference.first.isEmpty() && !m_reference.second.isEmpty();
    }
private:
    QPair<QString, QString> m_source, m_reference;
};

Q_DECLARE_METATYPE(ColumnConstraintForeignKey)

class ColumnConstraint
{
    Q_ENUMS(ConstraintType)
    Q_FLAGS(ConstraintType ConstraintTypes)
public:
    enum ConstraintType {CT_Unknown = 0, CT_PrimaryKey = 0x1, CT_NotNull = 0x2, CT_Unique = 0x4, CT_Default = 0x8, CT_Check = 0x10, CT_ForeignKey = 0x20, CT_Last = 0x40};
    Q_DECLARE_FLAGS(ConstraintTypes, ConstraintType)
    ColumnConstraint(const QString name = "", const ConstraintType type = CT_Unknown, const QVariant& data = QVariant());
    ColumnConstraint(const ColumnConstraint& old);
    ~ColumnConstraint();

    inline const ConstraintType type() const {return m_type;}
    void setType(const ConstraintType& newType);
    inline const QString& name() const {return m_name;}
    void setName(const QString& newName);
    inline const QVariant& data() const {return m_data;}
    void setData(const QVariant& newData);

    const QString getUMLConstraintString() const;
private:
    ConstraintType m_type;
    QVariant m_data;
    QString m_name;
};

typedef ColumnConstraint * PColumnConstraint;
typedef QSharedPointer<ColumnConstraint> SharedColumnConstraint;
Q_DECLARE_OPERATORS_FOR_FLAGS(ColumnConstraint::ConstraintTypes)

class ColumnConstraints: public QList<SharedColumnConstraint>
{
public:
    ColumnConstraints();

    void addConstraint(PColumnConstraint constraint);
    void deleteConstraint(int index);

    inline const ColumnConstraint::ConstraintTypes types() const {return m_types;}
    inline bool isConstraintType(ColumnConstraint::ConstraintType type) const {return m_types.testFlag(type);}
    PColumnConstraint constraint(const ColumnConstraint::ConstraintType type) const;
private:
    ColumnConstraint::ConstraintTypes m_types;
};

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

    inline const ColumnConstraints& constraints() const {return m_constraints;}
    inline void addConstraint(PColumnConstraint constraint) {m_constraints.addConstraint(constraint);}
    inline void deleteConstraint(int index) {m_constraints.deleteConstraint(index);}
    inline bool isConstraintType(ColumnConstraint::ConstraintType type) const {return m_constraints.isConstraintType(type);}

    const QString getUMLColumnPrefix() const;
    const QString getUMLColumnDescription() const;
private:
    PTableModel m_parent;
    QString  m_columnName;
    QString  m_columnComment;
    DataType m_dataType;
    ColumnConstraints m_constraints;

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

    inline int getAmountForType(const ColumnConstraint::ConstraintType type) const {return m_constraintCounters[type];}
    PColumnModel getColumnByName(const QString& columnName) const;
    void getColumnsForConstraintType(const ColumnConstraint::ConstraintType type, QList<PColumnModel>& result) const;
private:
    int m_constraintCounters[ColumnConstraint::CT_Last];
};

#endif //COLUMN_H
