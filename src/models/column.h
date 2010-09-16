#ifndef COLUMN_H
#define COLUMN_H

#include <QtCore/QList>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QVariant>

#include "datatypes.h"

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
public:
    inline const QString name() const {return m_columnName;}
    void setName(const QString& name);
    inline const QString comment() const {return m_columnComment;}
    void setComment(const QString& comment);
    inline const DataType& dataType() const {return m_dataType;}
    void setDataType(const DataType& dataType);

    inline const ColumnConstraints& constraints() const {return m_constraints;}
    inline void addConstraint(ColumnConstraint * constraint) {m_constraints.addConstraint(constraint);}
    inline void deleteConstraint(int index) {m_constraints.deleteConstraint(index);}
    inline bool isConstraintType(ColumnConstraint::ConstraintType type) const {return m_constraints.isConstraintType(type);}

    const QString getUMLColumnPrefix() const;
    const QString getUMLColumnDescription() const;
private:
    QString  m_columnName;
    QString  m_columnComment;
    DataType m_dataType;
    ColumnConstraints m_constraints;
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
