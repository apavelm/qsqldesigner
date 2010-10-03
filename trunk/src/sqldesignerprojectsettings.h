#ifndef SQLDESIGNERPROJECTSETTINGS_H
#define SQLDESIGNERPROJECTSETTINGS_H

#include <QtCore/QString>

class SqlDesignerProjectSettings
{
public:
    explicit SqlDesignerProjectSettings(const QString& projectName, const QString& dbmsType);
    ~SqlDesignerProjectSettings() {}

    inline const QString& name() const {return m_projectName;}
    inline const QString& dbmsType() const {return m_dbmsType;}

    void rename(const QString& newName);
private:
    QString m_projectName;
    QString m_dbmsType;
};

#endif // SQLDESIGNERPROJECTSETTINGS_H
