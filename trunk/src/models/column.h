#ifndef COLUMN_H
#define COLUMN_H

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QVariant>

#include "datatypes.h"

class ColumnConstraint
{
    Q_ENUMS(ConstraintType)
    Q_FLAGS(ConstraintType ConstraintTypes)
public:
    enum ConstraintType {CT_Unknown = 0, CT_PrimaryKey = 1, CT_NotNull = 2, CT_Unique = 4, CT_Default = 8, CT_Check = 16, CT_ForeignKey = 32, CT_Last = 7};
    Q_DECLARE_FLAGS(ConstraintTypes, ConstraintType)
    ColumnConstraint(const QString name, const ConstraintType type = CT_Unknown, const QVariant& data = QVariant());

    inline const ConstraintType type() const {return m_type;}
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

Q_DECLARE_OPERATORS_FOR_FLAGS(ColumnConstraint::ConstraintTypes)

class ColumnConstraints: private QList<ColumnConstraint>
{
public:
    ColumnConstraints();

    void addConstraint(ColumnConstraint * constraint);
    void deleteConstraint(int index);

    inline const ColumnConstraint::ConstraintTypes types() const {return m_types;}
    inline bool isPrimaryKey() const {return m_types.testFlag(ColumnConstraint::CT_PrimaryKey);}
    inline bool isNotNull() const {return m_types.testFlag(ColumnConstraint::CT_NotNull);}
    inline bool isUnique() const {return m_types.testFlag(ColumnConstraint::CT_Unique);}
    inline bool isDefaultConstraint() const {return m_types.testFlag(ColumnConstraint::CT_Default);}
    inline bool isCheckConstraint() const {return m_types.testFlag(ColumnConstraint::CT_Check);}
    inline bool isForeignKey() const {return m_types.testFlag(ColumnConstraint::CT_ForeignKey);}

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
    // TODO: datatype GET/SET

    inline const ColumnConstraints& constraints() const {return m_constraints;}
    inline void addConstraint(ColumnConstraint * constraint) {m_constraints.addConstraint(constraint);}
    inline void deleteConstraint(int index) {m_constraints.deleteConstraint(index);}
    inline bool isPrimaryKey() const {return m_constraints.isPrimaryKey();}
    inline bool isNotNull() const {return m_constraints.isNotNull();}
    inline bool isUnique() const {return m_constraints.isUnique();}
    inline bool isDefaultConstraint() const {return m_constraints.isDefaultConstraint();}
    inline bool isCheckConstraint() const {return m_constraints.isCheckConstraint();}
    inline bool isForeignKey() const {return m_constraints.isForeignKey();}

    const QString getUMLColumnPrefix() const;
    const QString getUMLColumnDescription() const;
private:
    QString  m_columnName;
    QString  m_columnComment;
    DataType m_dataType;
    ColumnConstraints m_constraints;
};

class ColumnList: public QList<ColumnModel>
{
public:
    ColumnList();
    void addColumn(const ColumnModel& column);

    inline int getAmountForType(const ColumnConstraint::ConstraintType type) const {return m_constraintCounters[type];}
    inline int getPrimaryKeyAmount() const {return getAmountForType(ColumnConstraint::CT_PrimaryKey);}
    inline int getNotNullAmount() const {return getAmountForType(ColumnConstraint::CT_NotNull);}
    inline int getUniqueAmount() const {return getAmountForType(ColumnConstraint::CT_Unique);}
    inline int getCheckAmount() const {return getAmountForType(ColumnConstraint::CT_Check);}
    inline int getForeignKeyAmount() const {return getAmountForType(ColumnConstraint::CT_ForeignKey);}
private:
    int m_constraintCounters[ColumnConstraint::CT_Last];
};

#endif //COLUMN_H
