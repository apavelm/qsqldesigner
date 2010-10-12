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
#include <QtCore/QVariant>

#include "sqldesignerproject.h"
#include "sqldesignerprojectsettings.h"
#include "models/modelmanager.h"
#include "widgets/widgetmanager.h"
#include "widgets/tablewidget.h"
#include "models/column.h"
#include "models/table.h"

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
        QDomNode settingsNode = nodeFromPrjectSettings(doc, project->settings());
        QDomNode graphicsNode = nodeFromGraphicsModel(doc, project->widgetManager());

        mainScope.appendChild(settingsNode);
        mainScope.appendChild(graphicsNode);
        doc.appendChild(mainScope);
        doc.save(out, IdentSize, QDomNode::EncodingFromTextStream);
        return true;
    }
    return false;
}

QDomNode XmlHelper::nodeFromPrjectSettings(QDomDocument& doc, PSqlDesignerProjectSettings settings)
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

QDomNode XmlHelper::nodeFromGraphicsModel(QDomDocument& doc, PWidgetManager wm)
{
    QDomNode graphicsNode = doc.createElement("graphics");
    QMap<QString, SharedTableWidget> tablesWidgets = wm->tablesWidgets();
    QMap<QString, SharedTableWidget>::iterator i = tablesWidgets.begin();
    QMap<QString, int> links;
    int linkNo = 0;

    while (i != tablesWidgets.end())
    {
        if (!(*i)->model()->hasForeignKeys())
        {
            links.insert(i.key(), linkNo);
            QDomNode table = nodeFromGraphicsTableWidget(doc, i->data(), linkNo++, links);
            graphicsNode.appendChild(table);
            i = tablesWidgets.erase(i);
        }
        else
        {
            // skiping table with FKs
            i++;
        }
    }

    // tables with FK
    i = tablesWidgets.begin();
    while (tablesWidgets.count() != 0)
    {
        if (isAllRefInList((*i)->model()->refTables(), links.keys()))
        {
            links.insert(i.key(), linkNo);
            QDomNode table = nodeFromGraphicsTableWidget(doc, i->data(), linkNo++, links);
            graphicsNode.appendChild(table);
            i = tablesWidgets.erase(i);
        }
        else
        {
            i++;
        }
        if (i == tablesWidgets.end())
        {
            i = tablesWidgets.begin();
        }
    }

    return graphicsNode;
}

QDomNode XmlHelper::nodeFromGraphicsTableWidget(QDomDocument& doc, PTableWidget table, int linkNo, const QMap<QString, int>& dict)
{
    QDomNode tableNode = doc.createElement("table");
    QDomElement tableElem = doc.createElement("properties");
    tableElem.setAttribute("x", table->pos().x());
    tableElem.setAttribute("y", table->pos().y());
    tableElem.setAttribute("name", table->name());
    tableElem.setAttribute("id", linkNo);
    // model describe
    PTableModel model = table->model();
    QDomNode columnsNode = doc.createElement("columns");
    const ColumnList& columnList = model->columns();
    foreach (const SharedColumnModel& c, columnList)
    {
        QDomElement columnElem = doc.createElement("column");
        columnElem.setAttribute("name", c->name());
        columnElem.setAttribute("datatype", c->dataType()->typeName());
        if (c->dataType()->parametersAmount() > 0)
        {
            QDomElement dataTypeParams = doc.createElement("datatype_parameters");
            dataTypeParams.setAttribute("l", c->dataTypeParameters().first);
            dataTypeParams.setAttribute("p", c->dataTypeParameters().second);
            columnElem.appendChild(dataTypeParams);
        }
        if (!c->comment().isEmpty())
        {
            QDomNode columnCommentNode = doc.createElement("comment");
            QDomText columnComment = doc.createTextNode("comment");
            columnComment.setData(c->comment());
            columnCommentNode.appendChild(columnComment);
            columnElem.appendChild(columnCommentNode);
        }
        const Constraints& constraintsList = c->constraints();
        if (constraintsList.count() > 0)
        {
            QDomNode constraintsNode = doc.createElement("constraints");
            foreach (const SharedConstraint& cn, constraintsList)
            {
                if (cn->type() != Constraint::CT_Unknown)
                {
                    QDomElement cnElem = doc.createElement("constraint");
                    cnElem.setAttribute("type", cn->type());
                    if (!cn->name().isEmpty())
                    {
                        cnElem.setAttribute("name", cn->name());
                    }
                    if (cn->data().isValid())
                    {
                        QDomNode cnDataNode = doc.createElement("constraint_data");
                        QVariant var = cn->data();
                        if (cn->type() == Constraint::CT_ForeignKey)
                        {
                            if (var.canConvert<ConstraintForeignKey>())
                            {
                                ConstraintForeignKey fk = var.value<ConstraintForeignKey>();
                                QDomElement fkTable = doc.createElement("reference_table");
                                fkTable.setAttribute("value", fk.referenceTable());
                                if (dict.contains(fk.referenceTable()))
                                {
                                    int id = dict.value(fk.referenceTable());
                                    fkTable.setAttribute("id", id);
                                }
                                cnDataNode.appendChild(fkTable);
                                foreach (const QString& col, fk.sourceColumns())
                                {
                                    QDomElement fk_src_col = doc.createElement("source_column");
                                    fk_src_col.setAttribute("value", col);
                                    cnDataNode.appendChild(fk_src_col);
                                }
                                foreach (const QString& col, fk.referenceColumns())
                                {
                                    QDomElement fk_ref_col = doc.createElement("reference_column");
                                    fk_ref_col.setAttribute("value", col);
                                    cnDataNode.appendChild(fk_ref_col);
                                }
                            }
                        }
                        else
                        {
                            QDomText cnStringData = doc.createTextNode("constraint_data");
                            cnStringData.setData(var.toString());
                            cnDataNode.appendChild(cnStringData);
                        }
                        cnElem.appendChild(cnDataNode);
                    }
                    constraintsNode.appendChild(cnElem);
                }
            }
            columnElem.appendChild(constraintsNode);
        }

        columnsNode.appendChild(columnElem);
    }

    tableNode.appendChild(tableElem);
    tableNode.appendChild(columnsNode);
    return tableNode;
}

bool XmlHelper::isAllRefInList(const QStringList& list, const QStringList& dict)
{
    foreach (const QString& s, list)
    {
        if (!dict.contains(s, Qt::CaseInsensitive))
            return false;
    }
    return true;
}