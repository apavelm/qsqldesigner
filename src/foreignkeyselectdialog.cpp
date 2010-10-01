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

#include "foreignkeyselectdialog.h"

#include "models/table.h"
#include "widgets/columnpreviewwidget.h"

#include "ui_foreignkeyselectdialog.h"

ForeignKeySelectDialog::ForeignKeySelectDialog(PColumnModel column, QWidget * parent, PSqlDesignerProject project) : QDialog(parent), ui(new Ui::ForeignKeySelectDialog), m_column(column)
{
    ui->setupUi(this);

    m_project = project;
    m_model = new ForeignKeySelectionViewModel();
    ui->columnView->setModel(m_model);
    ColumnPreviewWidget * previewWidget = new ColumnPreviewWidget(ui->columnView, m_model);
    connect(ui->columnView, SIGNAL(updatePreviewWidget(QModelIndex)), previewWidget, SLOT(onUpdatePreviewWidget(QModelIndex)));
    ui->columnView->setPreviewWidget(previewWidget);
}

ForeignKeySelectDialog::~ForeignKeySelectDialog()
{
    delete m_model;
    delete ui;
}

void ForeignKeySelectDialog::accept()
{
    QString tableRef, columnRef;
    const QModelIndexList& idxList =  ui->columnView->selectionModel()->selectedIndexes();

    foreach (const QModelIndex& idx, idxList)
    {
        QStandardItem * item = m_model->itemFromIndex(idx);
        QStandardItem * parent = item->parent();
        tableRef = parent->text();
        columnRef = item->text();
    }

    QVariant variant;
    // TODO: FIXME: replace creation of simple FK.
    variant.setValue(ConstraintForeignKey(tableRef, m_column->name(), columnRef));
    m_column->addConstraint(new Constraint(m_column, Constraint::CT_ForeignKey, variant));
    QDialog::accept();
}
