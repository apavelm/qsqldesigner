#ifndef DATATYPES_H
#define DATATYPES_H

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>

struct DataType
{
    QString typeName;
    QString sqlTypeAcronim;
    bool    canBeArray;
    bool    canIncrement;
};

typedef QList<DataType> DataTypes;
typedef QMap<QString, DataTypes> AllDatabaseDataTypes;

#endif //DATATYPES_H
