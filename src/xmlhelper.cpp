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

#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QVariant>

#include "sqldesignerproject.h"
#include "sqldesignerprojectsettings.h"
#include "models/modelmanager.h"
#include "widgets/widgetmanager.h"
#include "widgets/tablewidget.h"
#include "models/column.h"
#include "models/table.h"

#include "pluginmanager.h"

XmlHelper::XmlHelper()
{
}

XmlHelper::~XmlHelper()
{
}

PSqlDesignerProject XmlHelper::read(const QString& fileName)
{
    if (fileName.isEmpty())
    {
        return 0;
    }

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        return 0;
    }

    QXmlStreamReader reader(&file);
    PSqlDesignerProject rslt = 0;
    PSqlDesignerProjectSettings settings = 0;

    if (reader.readNextStartElement())
    {
        QString r_name = reader.name().toString();
        QString r_version = reader.attributes().value("version").toString();
        if (r_name == "sqldesigner_project" && isVersionAcceptable(r_version))
        {
            while (reader.readNextStartElement())
            {
                if (reader.name() == "settings")
                {
                    settings = readSettings(&reader);
                    settings->setFileName(fileName);
                }
                else
                if (reader.name() == "diagram")
                {
                    rslt = readDiagram(&reader, settings);
                }
                else
                    reader.skipCurrentElement();
            }
        }
        else
        {
            reader.raiseError(QObject::tr("The file is not an %1 project version %2 file.").arg(qApp->applicationName()).arg(qApp->applicationVersion()));
        }
    }

    if (!rslt)
    {
        if (settings)
        {
            delete settings;
        }
    }

    reader.clear();
    return rslt;
}

bool XmlHelper::save(const QString& fileName, PSqlDesignerProject project)
{
    if (project)
    {
        QFile file(fileName);
        if (!file.open(QFile::WriteOnly | QFile::Text))
        {
            return false;
        }

        QTextStream out(&file);
        out.setCodec("UTF-8");
        QDomDocument doc(project->name());
        QDomNode mainScope = doc.createElement("sqldesigner_project");
        mainScope.toElement().setAttribute("version", qApp->applicationVersion());
        QDomNode settingsNode = nodeFromPrjectSettings(doc, project->settings());
        QDomNode diagramNode = nodeFromModel(doc, project->widgetManager());
        mainScope.appendChild(settingsNode);
        mainScope.appendChild(diagramNode);
        doc.appendChild(mainScope);
        doc.save(out, IdentSize, QDomNode::EncodingFromTextStream);
        file.close();
        project->settings()->setFileName(fileName);
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

QDomNode XmlHelper::nodeFromModel(QDomDocument& doc, PWidgetManager wm)
{
    QDomNode diagramNode = doc.createElement("diagram");
    QMap<QString, SharedTableWidget> tablesWidgets = wm->tablesWidgets();
    QMap<QString, SharedTableWidget>::iterator i = tablesWidgets.begin();
    QMap<QString, int> links;
    int linkNo = 0;

    while (i != tablesWidgets.end())
    {
        if (!(*i)->model()->hasForeignKeys())
        {
            links.insert(i.key(), linkNo);
            QDomNode table = nodeFromTableWidget(doc, i->data(), linkNo++, links);
            diagramNode.appendChild(table);
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
            QDomNode table = nodeFromTableWidget(doc, i->data(), linkNo++, links);
            diagramNode.appendChild(table);
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

    return diagramNode;
}

QDomNode XmlHelper::nodeFromTableWidget(QDomDocument& doc, PTableWidget table, int linkNo, const QMap<QString, int>& dict)
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

PSqlDesignerProjectSettings XmlHelper::readSettings(QXmlStreamReader * reader)
{
    PSqlDesignerProjectSettings rslt = 0;
    QString projName;
    QString projDBMS;
    while (reader->readNextStartElement())
    {
        if (reader->name() == "project_name")
        {
            projName = reader->attributes().value("value").toString();
            reader->readNext();
        }
        else if (reader->name() == "project_dbmstype")
        {
            projDBMS = reader->attributes().value("value").toString();
            reader->readNext();
        }
        else
        {
            reader->skipCurrentElement();
        }
    }

    if (!projName.isEmpty() && !projDBMS.isEmpty())
    {
        rslt = new SqlDesignerProjectSettings(projName, projDBMS);
    }
    return rslt;
}

PTableModel XmlHelper::readTable(QXmlStreamReader * reader, PModelManager mm, const QString& dbmsType, QList<QPair<QString, QPointF> >& coords)
{
    PTableModel table = new TableModel(mm);
    // for table
    qreal x = 0;
    qreal y = 0;
    QString tableName;
    int tableID = 0;
    while (reader->readNextStartElement())
    {
        if (reader->name() == "properties")
        {
            x = reader->attributes().value("x").toString().toFloat();
            y = reader->attributes().value("y").toString().toFloat();
            tableName = reader->attributes().value("name").toString();
            tableID = reader->attributes().value("id").toString().toInt();
            coords << qMakePair(tableName, QPointF(x, y));
            reader->readNext();
        }
        else
        if (reader->name() == "columns")
        {
            QString columnDatatype;
            QString columnName;
            QString columnComment;
            int l = 0, p = 0;
            while (reader->readNextStartElement())
            {
                if (reader->name() == "column")
                {
                    PColumnModel column = new ColumnModel(table);
                    columnDatatype = reader->attributes().value("datatype").toString();
                    columnName = reader->attributes().value("name").toString();

                    while (reader->readNextStartElement())
                    {
                        if (reader->name() == "comment")
                        {
                            columnComment = reader->readElementText();
                            reader->readNext();
                        }
                        else
                        if (reader->name() == "datatype_parameters")
                        {
                            l = reader->attributes().value("l").toString().toInt();
                            p = reader->attributes().value("p").toString().toInt();
                            reader->readNext();
                        }
                        else
                        if (reader->name() == "constraints")
                        {
                            while (reader->readNextStartElement())
                            {
                                if (reader->name() == "constraint")
                                {
                                    QString constraintName = reader->attributes().value("name").toString();
                                    Constraint::ConstraintType constraintType = (Constraint::ConstraintType)(reader->attributes().value("type").toString().toInt());
                                    QVariant varColumnConstraintData = QVariant();
                                    if (reader->readNextStartElement())
                                    {
                                        if (reader->name() == "constraint_data")
                                        {
                                            if (constraintType == Constraint::CT_ForeignKey)
                                            {
                                                QString refTable;
                                                int refTableID = 0;
                                                QList<QString> sourceColumns;
                                                QList<QString> refColumns;
                                                while (reader->readNextStartElement())
                                                {
                                                    if (reader->name() == "reference_table")
                                                    {
                                                        refTable = reader->attributes().value("value").toString();
                                                        refTableID = reader->attributes().value("id").toString().toInt();
                                                        reader->readNext();
                                                    }
                                                    else
                                                        if (reader->name() == "source_column")
                                                        {
                                                        sourceColumns << reader->attributes().value("value").toString();
                                                        reader->readNext();
                                                    }
                                                    else
                                                        if (reader->name() == "reference_column")
                                                        {
                                                        refColumns << reader->attributes().value("value").toString();
                                                        reader->readNext();
                                                    }
                                                    else
                                                    {
                                                        reader->skipCurrentElement();
                                                    }
                                                }
                                                varColumnConstraintData.setValue(ConstraintForeignKey(refTable, sourceColumns, refColumns));
                                            }
                                            else
                                            {
                                                varColumnConstraintData.setValue(reader->readElementText());
                                            }
                                        }
                                        reader->readNext();
                                    }
                                    PConstraint columnConstraint = new Constraint(column, constraintType, varColumnConstraintData);
                                    if (!constraintName.isEmpty())
                                    {
                                        columnConstraint->setName(constraintName);
                                    }
                                    column->addConstraint(columnConstraint);
                                    reader->readNext();
                                }
                                else
                                {
                                    reader->skipCurrentElement();
                                }
                            }
                        }
                        else
                        {
                            reader->skipCurrentElement();
                        }

                    }

                    column->setName(columnName);
                    column->setComment(columnComment);
                    column->setDataType(PLUGINMANAGER->dataTypesForDatabase(dbmsType).typeByName(columnDatatype));
                    column->setDataTypeParameters(qMakePair<int, int>(l, p));
                    table->addColumn(column);
                }
                else
                {
                    reader->skipCurrentElement();
                }
            }
        }
        else
        if (reader->name() == "constraints")
        {
            while (reader->readNextStartElement())
            {
                if (reader->name() == "constraint")
                {
                    QString constraintName = reader->attributes().value("name").toString();
                    Constraint::ConstraintType constraintType = (Constraint::ConstraintType)(reader->attributes().value("type").toString().toInt());
                    QVariant varTableConstraintData;
                    if (reader->readNextStartElement())
                    {
                        if (reader->name() == "constraint_data")
                        {
                            if (constraintType == Constraint::CT_ForeignKey)
                            {
                                QString refTable;
                                QList<QString> sourceColumns;
                                QList<QString> refColumns;
                                while (reader->readNextStartElement())
                                {
                                    if (reader->name() == "reference_table")
                                    {
                                        refTable = reader->attributes().value("value").toString();
                                    }
                                    else
                                        if (reader->name() == "source_column")
                                        {
                                        sourceColumns << reader->attributes().value("value").toString();
                                    }
                                    else
                                        if (reader->name() == "reference_column")
                                        {
                                        refColumns << reader->attributes().value("value").toString();
                                    }
                                    else
                                    {
                                        reader->skipCurrentElement();
                                    }
                                }
                                varTableConstraintData.setValue(ConstraintForeignKey(refTable, sourceColumns, refColumns));
                            }
                            else
                            {
                                varTableConstraintData.setValue(reader->readElementText());
                            }
                        }
                        reader->readNext();
                    }
                    PConstraint tableConstraint = new Constraint(table, constraintType, varTableConstraintData);
                    if (!constraintName.isEmpty())
                    {
                        tableConstraint->setName(constraintName);
                    }
                    table->addConstraint(tableConstraint);
                    reader->readNext();
                }
                else
                {
                    reader->skipCurrentElement();
                }
            }
        }
        else
        {
            reader->skipCurrentElement();
        }
    }
    table->setName(tableName);
    return table;
}

PSqlDesignerProject XmlHelper::readDiagram(QXmlStreamReader * reader, PSqlDesignerProjectSettings settings)
{
    PSqlDesignerProject rslt = 0;
    PModelManager mm = new ModelManager(0, settings);
    QList<QPair<QString, QPointF> >coords;
    while (reader->readNextStartElement())
    {
        if (reader->name() == "table")
        {
            PTableModel table = readTable(reader, mm, settings->dbmsType(), coords);
            if (table)
            {
                mm->addTable(table);
            }
        }
        else
        {
            reader->skipCurrentElement();
        }
    }
    rslt = new SqlDesignerProject(settings, mm, coords);
    mm->setParent(rslt);
    return rslt;
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

bool XmlHelper::isVersionAcceptable(const QString& version)
{
     QStringList sAppVersion = qApp->applicationVersion().split('.');
     QStringList sRequired = version.split('.');
     int countMax = qMax(sAppVersion.count(), sRequired.count());
     while (sAppVersion.count() != sRequired.count())
     {
         if (sAppVersion.count() > sRequired.count())
         {
             sRequired.append("0");
         }
         else
         {
             sAppVersion.append("0");
         }
     }
     for (int i = 0; i < countMax; i++)
     {
         int a = sAppVersion.at(i).toInt();
         int b = sRequired.at(i).toInt();
         if (b > a)
         {
             return false;
         }
     }
     return true;
}

QByteArray XmlHelper::serializeTableWidget(PTableWidget table)
{
    if (table)
    {
        QDomDocument doc("TableWidget");
        QByteArray rslt;
        QDomNode node = nodeFromTableWidget(doc, table);
        doc.appendChild(node);
        QTextStream out(&rslt, QIODevice::WriteOnly);
        doc.save(out, IdentSize);
        return rslt;
    }
    return QByteArray();
}

PTableModel XmlHelper::deserealizeTableWidget(const QByteArray& ba, PModelManager mm, const QString& dbmsType, QList<QPair<QString, QPointF> >& coords)
{
    if (!ba.isEmpty())
    {
        QXmlStreamReader reader(ba);
        while (reader.readNextStartElement())
        {
            if (reader.name() == "table")
            {
                return readTable(&reader, mm, dbmsType, coords );
            }
            else
            {
                reader.skipCurrentElement();
            }
        }

    }
    return 0;
}
