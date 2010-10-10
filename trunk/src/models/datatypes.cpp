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

#include "datatypes.h"

DataType::DataType(const QString name, int parametersCount, const QString relatives)
{
    m_typeName = name;
    m_parametersAmount = parametersCount;
    m_relativeTypes = relatives;
}

DataType::DataType(const DataType& other)
{
    m_typeName = other.typeName();
    m_relativeTypes = other.relativeTypes();
    m_parametersAmount = other.parametersAmount();
}

bool DataType::isRelative(DataType * other) const
{
    if (QString::compare(m_typeName, other->typeName(), Qt::CaseInsensitive) == 0)
        return true;

    QStringList relativeLst = m_relativeTypes.split(";");
    if (relativeLst.contains(other->typeName(), Qt::CaseInsensitive))
        return true;

    return false;
}

QString DataType::fullTypeName(const QPair<int, int> params) const
{
    QString dataTypeName = m_typeName;
    if (m_parametersAmount > 0)
    {
        dataTypeName += "(" + QString::number(params.first);
        if (m_parametersAmount > 1)
        {
            dataTypeName += "," + QString::number(params.second);
        }
        dataTypeName += ")";
    }
    return dataTypeName;
}

// DataTypes

DataTypes::DataTypes() : QList<QSharedPointer<DataType> >()
{

}

QStringList DataTypes::toStringList() const
{
    QStringList rslt;
    foreach (const SharedDataType& dt, *this)
    {
        rslt << dt->typeName();
    }
    rslt.sort();
    return rslt;
}

PDataType DataTypes::typeByName(const QString& typeName)
{
    foreach (const SharedDataType& dt, *this)
    {
        if (QString::compare(dt->typeName(), typeName, Qt::CaseInsensitive) == 0 )
        {
            return dt.data();
        }
    }
    return 0;
}
