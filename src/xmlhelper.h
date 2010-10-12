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

#ifndef XMLHELPER_H
#define XMLHELPER_H

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QIODevice>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>

class SqlDesignerProject;
typedef SqlDesignerProject * PSqlDesignerProject;
class SqlDesignerProjectSettings;
typedef SqlDesignerProjectSettings * PSqlDesignerProjectSettings;
class ModelManager;
typedef ModelManager * PModelManager;
class WidgetManager;
typedef WidgetManager * PWidgetManager;
class TableWidget;
typedef TableWidget * PTableWidget;

class XmlHelper
{
public:
    static bool read(QIODevice * device);
    static bool save(QIODevice * device, PSqlDesignerProject project);

private:
    static const int IdentSize = 4;
    XmlHelper();
    ~XmlHelper();

    static QDomNode nodeFromPrjectSettings(QDomDocument& doc, PSqlDesignerProjectSettings settings);
    static QDomNode nodeFromGraphicsModel(QDomDocument& doc, PWidgetManager wm);
    static QDomNode nodeFromGraphicsTableWidget(QDomDocument& doc, PTableWidget table, int linkNo, const QMap<QString, int>& dict);

    static bool isAllRefInList(const QStringList& list, const QStringList& dict);
};

#endif // XMLHELPER_H
