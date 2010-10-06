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
#include <QtCore/QPair>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QStringList>

class DataType
{
public:
    DataType(const QString name = QString(), int parametersCount = 0, const QString relatives = QString());
    DataType(const DataType& other);
    virtual ~DataType() {}

    inline const QString& typeName() const {return m_typeName;}
    inline const QString& relativeTypes() const {return m_relativeTypes;}
    inline int parametersAmount() const {return m_parametersAmount;}

    bool isRelative(DataType * other) const;
    QString fullTypeName(const QPair<int, int> params) const;
private:
    QString m_typeName;
    QString m_relativeTypes;
    int m_parametersAmount;
};

typedef DataType * PDataType;
typedef QSharedPointer<DataType> SharedDataType;

class DataTypes : public QList<SharedDataType>
{
public:
    DataTypes();
    ~DataTypes() {}

    QStringList toStringList() const;
    const PDataType typeByName(const QString& typeName);
};

typedef QMap<QString, DataTypes> AllDatabaseDataTypes;

#endif //DATATYPES_H
