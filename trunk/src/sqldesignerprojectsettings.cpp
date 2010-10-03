#include "sqldesignerprojectsettings.h"

SqlDesignerProjectSettings::SqlDesignerProjectSettings(const QString& projectName, const QString& dbmsType)
{
    m_projectName = projectName;
    m_dbmsType = dbmsType;
}

void SqlDesignerProjectSettings::rename(const QString& newName)
{
    if (newName.isEmpty())
        return;
    m_projectName = newName;
}
