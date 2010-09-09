#ifndef COLUMN_H
#define COLUMN_H

#include <QtCore/QString>

#include "datatypes.h"

class ColumnModel
{
public:
    bool isValid() const {return true;}
    QString getUMLColumnPrefix() const;
    QString getUMLColumnDescription() const;

    QString  columnName;
    QString  columnComment;
    DataType dataType;
    QString  defaultValue;
    bool     isPrimaryKey;
    bool     isNotNull;
    bool     isUnique;
private:
    bool stubIsFK() const {return false;}
};

#endif //COLUMN_H
