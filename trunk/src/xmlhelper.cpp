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

#include "xmlhelper.h"

#include <QtCore/QTextStream>

#include "sqldesignerproject.h"
#include "sqldesignerprojectsettings.h"
#include "models/modelmanager.h"
#include "widgets/widgetmanager.h"

XmlHelper::XmlHelper()
{
}

XmlHelper::~XmlHelper()
{
}

bool XmlHelper::read(QIODevice * device)
{
    QDomDocument doc;
    QString sError;
    int nErrorLine, nErrorColumn;
    if (!doc.setContent(device, true, &sError, &nErrorLine, &nErrorColumn))
        return false;

    QDomElement root = doc.documentElement();
    if ( root.tagName() != "sqldesigner_project")
        return false;

    // parse xml

    return true;
}

bool XmlHelper::save(QIODevice * device, PSqlDesignerProject project)
{
    if (project)
    {
        QTextStream out(device);
        out.setCodec("UTF-8");
        QDomDocument doc(project->name());
        QDomNode mainScope = doc.createElement("sqldesigner_project");
        QDomNode settingsNode = elementFromPrjectSettings(doc, project->settings());
        QDomNode modelNode = elementFromPrjectSettings(doc, project->modelManager());
        QDomNode graphicsNode = elementFromPrjectSettings(doc, project->widgetManager());

        mainScope.appendChild(settingsNode);
        mainScope.appendChild(modelNode);
        mainScope.appendChild(graphicsNode);
        doc.appendChild(mainScope);
        doc.save(out, IdentSize, QDomNode::EncodingFromTextStream);
        return true;
    }
    return false;
}

QDomNode XmlHelper::elementFromPrjectSettings(QDomDocument& doc, PSqlDesignerProjectSettings settings)
{
    QDomNode settingsNode = doc.createElement("settings");
    QDomElement settingsName = doc.createElement("project_name");
    QDomElement settingsDBMS = doc.createElement("project_dbmstype");

    settingsName.setAttribute("value", settings->name());
    settingsDBMS.setAttribute("value", settings->dbmsType());

    settingsNode.appendChild(settingsName);
    settingsNode.appendChild(settingsDBMS);
    return settingsNode;
}

QDomNode XmlHelper::elementFromModelManager(QDomDocument& doc, PModelManager mm)
{
    QDomNode modelNode = doc.createElement("model");

    return modelNode;
}

QDomNode XmlHelper::elementFromWidgetManager(QDomDocument& doc, PWidgetManager wm)
{
    QDomNode graphicsNode = doc.createElement("graphics");

    return graphicsNode;
}
