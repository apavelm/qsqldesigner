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

#include "columnpreviewwidget.h"
#include "../Simpleforeignkeyselectionviewmodel.h"
#include "../models/modelmanager.h"
#include "../projectmanager.h"

#include "ui_columnpreviewwidget.h"

ColumnPreviewWidget::ColumnPreviewWidget(QWidget *parent, SimpleForeignKeySelectionViewModel *model) : QWidget(parent), m_model(model), ui(new Ui::ColumnPreviewWidget)
{
    ui->setupUi(this);
}

ColumnPreviewWidget::~ColumnPreviewWidget()
{
    delete ui;
}

void ColumnPreviewWidget::onUpdatePreviewWidget(QModelIndex index)
{
    if (m_model)
    {
        QStandardItem * item = m_model->itemFromIndex(index);
        if (item)
        {
            QStandardItem * parent = item->parent();
            if (parent)
            {
                const QString tableName = parent->text();
                const QString columnName = item->text();

                PColumnModel column = CURRENTPROJECT->modelManager()->getColumnByName(tableName, columnName);

                ui->lblColumnName->setText(column->name());
                const QString & comment = column->comment();
                if (comment.isEmpty())
                {
                    ui->labelComment->hide();
                }
                else
                {
                    ui->labelComment->show();
                }
                ui->lblColumnComment->setText(comment);
                ui->lblColumnDatatype->setText(""); // TODO: insert DATATYPE serialization string
             }
        }
    }
    else
        clearContents();
}

void ColumnPreviewWidget::clearContents()
{
    ui->lblColumnName->clear();
    ui->lblColumnComment->clear();
    ui->lblColumnDatatype->clear();
}
