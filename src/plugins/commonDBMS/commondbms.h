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

#ifndef COMMONDBMSPLUGIN_H
#define COMMONDBMSPLUGIN_H

#include <QtCore/QObject>
#include <QtCore/QtPlugin>

#include "../../models/plugindbmsinterface.h"

class CommonDBMSPlugin : public QObject, public PluginDBMSInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginDBMSInterface)
public:
    CommonDBMSPlugin();
    ~CommonDBMSPlugin();

    inline const QString databaseName() const { return "Common Database"; }
    inline int internalVersion() const { return 1; }
    inline const DataTypes& dataTypes() const {return m_dataTypeList;}

    QString generateDDL(PModelManager model) const;
private:
    DataTypes m_dataTypeList;

    QString generateDDL_Table(PTableModel pTable) const;
    QString generateDDL_Column(PColumnModel pColumn) const;
    QString generateDDL_TableConstraint(PConstraint pConstraint) const;
};

#endif // COMMONDBMSPLUGIN_H
