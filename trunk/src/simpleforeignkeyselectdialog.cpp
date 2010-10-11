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

#include "simpleforeignkeyselectdialog.h"

#include "models/table.h"
#include "widgets/columnpreviewwidget.h"

#include "ui_simpleforeignkeyselectdialog.h"

SimpleForeignKeySelectDialog::SimpleForeignKeySelectDialog(PColumnModel column, QWidget * parent) :
        QDialog(parent),
        ui(new Ui::SimpleForeignKeySelectDialog),
        m_model(new SimpleForeignKeySelectionViewModel(this, column->dataType())),
        m_column(column),
        m_constraint(0)
{
    ui->setupUi(this);

    ui->columnView->setModel(m_model.data());
    ColumnPreviewWidget * previewWidget = new ColumnPreviewWidget(ui->columnView, m_model.data());
    connect(ui->columnView, SIGNAL(updatePreviewWidget(QModelIndex)), previewWidget, SLOT(onUpdatePreviewWidget(QModelIndex)));
    ui->columnView->setPreviewWidget(previewWidget);
}

SimpleForeignKeySelectDialog::~SimpleForeignKeySelectDialog()
{
    m_constraint = 0;
    delete ui;
}

bool SimpleForeignKeySelectDialog::selectItem(const QModelIndex& index)
{
    if (index.isValid())
    {
        QStandardItem * item = m_model->itemFromIndex(index);
        if (item)
        {
            QStandardItem * parent = item->parent();
            if (parent)
            {
                QString tableRef = parent->text();
                QString columnRef = item->text();

                QVariant variant;
                // Creating simple FK.
                variant.setValue(ConstraintForeignKey(tableRef, m_column->name(), columnRef));
                m_constraint = new Constraint(m_column, Constraint::CT_ForeignKey, variant);
                m_column->addConstraint(m_constraint);
                return true;
            }
        }
    }
    return false;
}

void SimpleForeignKeySelectDialog::accept()
{
    const QModelIndexList& idxList =  ui->columnView->selectionModel()->selectedIndexes();

    if (idxList.count() > 0)
    {
        if (selectItem(idxList.at(0)))
        {
            QDialog::accept();
        }
    }
    else
    {
        QDialog::reject();
    }

}

void SimpleForeignKeySelectDialog::on_columnView_doubleClicked(QModelIndex index)
{
    accept();
}
