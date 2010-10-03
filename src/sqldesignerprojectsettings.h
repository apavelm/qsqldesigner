/***************************************************************************
 *   Copyright (C) 2010 by Pavel Andreev                                   *
 *   Mail: apavelm on gmail point com (apavelm@gmail.com)                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, you can read it                      *
 *   here <http://www.gnu.org/licenses/>, or write to the                  *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

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
