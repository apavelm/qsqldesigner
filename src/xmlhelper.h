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

#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QPair>
#include <QtCore/QPointF>
#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QXmlStreamReader>

class SqlDesignerProject;
typedef SqlDesignerProject * PSqlDesignerProject;
class SqlDesignerProjectSettings;
typedef SqlDesignerProjectSettings * PSqlDesignerProjectSettings;
class TableModel;
typedef TableModel * PTableModel;
class ModelManager;
typedef ModelManager * PModelManager;
class WidgetManager;
typedef WidgetManager * PWidgetManager;
class TableWidget;
typedef TableWidget * PTableWidget;

class XmlHelper
{
public:
    static PSqlDesignerProject read(const QString& fileName);
    static bool save(const QString& fileName, PSqlDesignerProject project);

    static QByteArray serializeTableWidget(PTableWidget table);
    static PTableModel deserealizeTableWidget(const QByteArray& ba, PModelManager mm, const QString& dbmsType, QList<QPair<QString, QPointF> >& coords);
private:
    static const int IdentSize = 4;
    XmlHelper();
    ~XmlHelper();

    static PSqlDesignerProjectSettings readSettings(QXmlStreamReader * reader);
    static PSqlDesignerProject readDiagram(QXmlStreamReader * reader, PSqlDesignerProjectSettings settings);
    static PTableModel readTable(QXmlStreamReader * reader, PModelManager mm, const QString& dbmsType, QList<QPair<QString, QPointF> >& coords);

    static QDomNode nodeFromPrjectSettings(QDomDocument& doc, PSqlDesignerProjectSettings settings);
    static QDomNode nodeFromModel(QDomDocument& doc, PWidgetManager wm);
    static QDomNode nodeFromTableWidget(QDomDocument& doc, PTableWidget table, int linkNo = 0, const QMap<QString, int>& dict = QMap<QString, int>());

    static bool isAllRefInList(const QStringList& list, const QStringList& dict);
    static bool isVersionAcceptable(const QString& version);
};

#endif // XMLHELPER_H
