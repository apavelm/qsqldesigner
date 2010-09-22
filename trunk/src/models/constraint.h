#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QVariant>

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

class Constraint
{
    Q_ENUMS(ConstraintType)
    Q_FLAGS(ConstraintType ConstraintTypes)
public:
    enum ConstraintType {CT_Unknown = 0, CT_PrimaryKey = 0x1, CT_NotNull = 0x2, CT_Unique = 0x4, CT_Default = 0x8, CT_Check = 0x10, CT_ForeignKey = 0x20, CT_Last = 0x40};
    Q_DECLARE_FLAGS(ConstraintTypes, ConstraintType)
    Constraint(const ConstraintType type = CT_Unknown, const QVariant& data = QVariant());
    Constraint(const Constraint& old);
    ~Constraint();

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

typedef Constraint * PConstraint;
typedef QSharedPointer<Constraint> SharedConstraint;
Q_DECLARE_OPERATORS_FOR_FLAGS(Constraint::ConstraintTypes)

class Constraints: public QList<SharedConstraint>
{
public:
    Constraints();

    void addConstraint(PConstraint constraint);
    void deleteConstraint(int index);

    inline const Constraint::ConstraintTypes types() const {return m_types;}
    inline bool isConstraintType(Constraint::ConstraintType type) const {return m_types.testFlag(type);}
    PConstraint constraint(const Constraint::ConstraintType type) const;
private:
    Constraint::ConstraintTypes m_types;
};

#endif // CONSTRAINT_H
