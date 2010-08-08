#ifndef COLUMN_H
#define COLUMN_H

#include <QtCore/QString>

#include "datatypes.h"

struct ColumnModel
{
    QString  columnName;
    QString  columnComment;
    DataType dataType;
    QString  defaultValue;
    bool     isPrimaryKey;
    bool     isNotNull;
};

#endif //COLUMN_H
