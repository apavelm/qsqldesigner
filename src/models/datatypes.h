#ifndef DATATYPES_H
#define DATATYPES_H

#include <QtCore/QList>
#include <QtCore/QString>

struct DataType
{
    QString typeName;
    QString sqlTypeAcronim;
    bool    canBeArray;
};

typedef QList<DataType> DataTypes;

//static DataTypes loadDataTypes();

#endif //DATATYPES_H
