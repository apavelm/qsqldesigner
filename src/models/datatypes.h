#ifndef DATATYPES_H
#define DATATYPES_H

#include <QtCore/QList>
#include <QtCore/QString>

class DataType
{
public:
    DataType() {}
    QString typeName;
    QString sqlTypeAcronim;
    bool    canBeArray;
};

typedef QList<DataType> DataTypes;

//static DataTypes loadDataTypes();

#endif //DATATYPES_H
