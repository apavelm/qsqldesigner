#include "column.h"

QString ColumnModel::getUMLColumnPrefix() const
{
    QString rslt;
    if (isNotNull) rslt += "*";
    if (isPrimaryKey && stubIsFK())
    {
        rslt+="pfK";
    }
    else
    {
        if (isPrimaryKey)
        {
            rslt += "PK";
        }
        if (stubIsFK())
        {
            rslt += "PK";
        }
    }

    return rslt;
}

QString ColumnModel::getUMLColumnDescription() const
{
    QString rslt = columnName + ": " + "datatype";
    return rslt;
}
