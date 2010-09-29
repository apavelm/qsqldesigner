#include "commondatatypes.h"

CommonDataTypesPlugin::CommonDataTypesPlugin()
{
    DataType dt;
        dt.typeName = "INTEGER";
        dt.sqlTypeAcronim = "INT";
        dt.canBeArray = false;
        dt.canIncrement = true;
        m_dataypeList << dt;

        dt.typeName = "BIG INTEGER";
        dt.sqlTypeAcronim = "BIGINT";
        dt.canBeArray = false;
        dt.canIncrement = true;
        m_dataypeList << dt;

        dt.typeName = "CHARACTER";
        dt.sqlTypeAcronim = "CHAR";
        dt.canBeArray = true;
        dt.canIncrement = false;
        m_dataypeList << dt;

        dt.typeName = "VARYING CHARACTER";
        dt.sqlTypeAcronim = "VARCHAR";
        dt.canBeArray = true;
        dt.canIncrement = false;
        m_dataypeList << dt;

        dt.typeName = "DATE AND TIME";
        dt.sqlTypeAcronim = "DATETIME";
        dt.canBeArray = false;
        dt.canIncrement = false;
        m_dataypeList << dt;
}

Q_EXPORT_PLUGIN2(baseTypes, CommonDataTypesPlugin);
