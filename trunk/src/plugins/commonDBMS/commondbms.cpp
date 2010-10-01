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

#include "commondbms.h"

CommonDBMSPlugin::CommonDBMSPlugin()
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

Q_EXPORT_PLUGIN2(commonDBMS, CommonDBMSPlugin);
