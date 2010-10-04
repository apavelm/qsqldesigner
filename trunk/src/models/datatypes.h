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

#ifndef DATATYPES_H
#define DATATYPES_H

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>

struct DataType
{
    QString typeName;
    bool    canBeArray;
    bool    canIncrement;
    QString relativeTypes;
    int     parametersAmount;
};

class DataTypes : public QList<DataType>
{
public:
    DataTypes() : QList<DataType>() {}

    QStringList toStringList()
    {
        QStringList rslt;
        foreach (const DataType& dt, *this)
        {
            rslt << dt.typeName;
        }
        return rslt;
    }

    const DataType typeByName(const QString& typeName)
    {
        foreach (const DataType& dt, *this)
        {
            if (QString::compare(dt.typeName, typeName, Qt::CaseInsensitive) == 0 )
            {
                return dt;
            }
        }
        return DataType();
    }

};

typedef QMap<QString, DataTypes> AllDatabaseDataTypes;

#endif //DATATYPES_H
