#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

class ColumnModel;
typedef ColumnModel * PColumnModel;

class ConstraintForeignKey
{
public:
    enum { Type = QVariant::UserType + 2 };
    inline int type() const {return Type;}

    ConstraintForeignKey() {}

    ConstraintForeignKey(const QString& tableRef, const QList<QString>& columnsSource, const QList<QString>& columnsRef)
    {
        m_referenceTable = tableRef;
        m_sourceColumns = columnsSource;
        m_referenceColumns = columnsRef;
    }

    // constructor for simple FK
    ConstraintForeignKey(const QString& tableRef, const QString& columnSource, const QString& columnRef)
    {
        m_referenceTable = tableRef;
        m_sourceColumns.append(columnSource);
        m_referenceColumns.append(columnRef);
    }

    ConstraintForeignKey(const ConstraintForeignKey& old)
    {
        if (old.isValid())
        {
            m_referenceTable = old.referenceTable();
            m_sourceColumns = old.sourceColumns();
            m_referenceColumns = old.referenceColumns();
        }
    }

    inline const QString referenceTable() const {return m_referenceTable;}
    inline const QList<QString>& referenceColumns() const {return m_referenceColumns;}
    inline const QList<QString>& sourceColumns() const {return m_sourceColumns;}

    inline bool isSimple() const
    {
        return m_sourceColumns.count() == 1 && m_referenceColumns.count() == 1;
    }

    inline bool isValid() const
    {
        return !m_referenceTable.isEmpty() && !m_referenceColumns.isEmpty() && !m_sourceColumns.isEmpty();
    }
private:
    QString m_referenceTable;
    QList<QString> m_sourceColumns;
    QList<QString> m_referenceColumns;
};

Q_DECLARE_METATYPE(ConstraintForeignKey)

class Constraint
{
    Q_ENUMS(ConstraintType)
    Q_FLAGS(ConstraintType ConstraintTypes)
public:
    enum ConstraintType {CT_Unknown = 0, CT_PrimaryKey = 0x1, CT_NotNull = 0x2, CT_Unique = 0x4, CT_Default = 0x8, CT_Check = 0x10, CT_ForeignKey = 0x20, CT_Last = 0x40};
    Q_DECLARE_FLAGS(ConstraintTypes, ConstraintType)
    Constraint(PColumnModel column, const ConstraintType type = CT_Unknown, const QVariant& data = QVariant());
    Constraint(const Constraint& old);
    ~Constraint();

    inline ConstraintType type() const {return m_type;}
    inline PColumnModel column() const {return m_column;}
    void setType(const ConstraintType& newType);
    inline const QString& name() const {return m_name;}
    void setName(const QString& newName);
    inline const QVariant& data() const {return m_data;}
    void setData(const QVariant& newData);

    const QString getUMLConstraintString() const;
private:
    PColumnModel m_column;
    ConstraintType m_type;
    QVariant m_data;
    QString m_name;

    QString defaultName(const ConstraintType type, const QVariant& var = QVariant());
};

typedef Constraint * PConstraint;
typedef QSharedPointer<Constraint> SharedConstraint;
Q_DECLARE_OPERATORS_FOR_FLAGS(Constraint::ConstraintTypes)

class Constraints: public QList<SharedConstraint>
{
public:
    Constraints();

    void addConstraint(PConstraint constraint);
    void deleteConstraint(PConstraint constraint);
    void deleteConstraint(int index);

    inline const Constraint::ConstraintTypes types() const {return m_types;}
    inline bool isConstraintType(Constraint::ConstraintType type) const {return m_types.testFlag(type);}
    PConstraint constraint(const Constraint::ConstraintType type) const;
private:
    Constraint::ConstraintTypes m_types;
};

#endif // CONSTRAINT_H
