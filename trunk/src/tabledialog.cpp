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

#include "columndialog.h"
#include "tabledialog.h"
#include "models/column.h"

#include "ui_tabledialog.h"

TableDialog::TableDialog(PTableModel table, QWidget * parent) :
            QDialog(parent),
            ui(new Ui::TableDialog),
            m_model(table)
{
    ui->setupUi(this);
    clearColumnsTable();
    ui->edtTableName->setText(m_model->name());
    // TODO: insert change UI according to TableModel
}

TableDialog::~TableDialog()
{
    delete ui;
}

void TableDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void TableDialog::on_columnAddButton_clicked()
{
    m_model->setName(ui->edtTableName->text());

    PColumnModel column = new ColumnModel(m_model);
    ColumnDialog dlg(column, this);
    if (dlg.exec() == QDialog::Accepted)
    {
        int row = ui->columnsTable->rowCount();
        ui->columnsTable->insertRow(row);
        QTableWidgetItem * item1 = new QTableWidgetItem((column->isConstraintType(Constraint::CT_PrimaryKey) ? "PK" : "" )); // PK
        item1->setTextAlignment(Qt::AlignHCenter);
        QTableWidgetItem * item2 = new QTableWidgetItem(column->name()); // Name
        item2->setTextAlignment(Qt::AlignLeft);
        QTableWidgetItem * item3 = new QTableWidgetItem(column->dataType()->fullTypeName(column->dataTypeParameters())); // Type
        item3->setTextAlignment(Qt::AlignHCenter);
        QTableWidgetItem * item4 = new QTableWidgetItem((column->isConstraintType(Constraint::CT_NotNull) ? "Yes" : "No" )); // Not Null
        item4->setTextAlignment(Qt::AlignHCenter);
        QTableWidgetItem * item5 = new QTableWidgetItem((column->isConstraintType(Constraint::CT_Unique) ? "Yes" : "No" )); // Unique
        item5->setTextAlignment(Qt::AlignHCenter);

        ui->columnsTable->setItem(row, 0, item1);
        ui->columnsTable->setItem(row, 1, item2);
        ui->columnsTable->setItem(row, 2, item3);
        ui->columnsTable->setItem(row, 3, item4);
        ui->columnsTable->setItem(row, 4, item5);
        ui->columnsTable->setCurrentItem(0);

        m_model->addColumn(column);
    }
    else
    {
        delete column;
    }
}

void TableDialog::accept()
{
    m_model->setName(ui->edtTableName->text());
    // TODO: insert UI to table settings
    QDialog::accept();
}

void TableDialog::on_columnEditButton_clicked()
{
    m_model->setName(ui->edtTableName->text());
    int selRow = ui->columnsTable->currentRow();
    QTableWidgetItem * item = ui->columnsTable->item(selRow, 1);

    if (item)
    {
        PColumnModel column = m_model->column(item->text());
        ColumnDialog dlg(column, this);
        if (dlg.exec() == QDialog::Accepted)
        {
            applyColumnToTableUI(selRow, column);
        }
    }
}

void TableDialog::on_columnDeleteButton_clicked()
{
    int selRow = ui->columnsTable->currentRow();
    ui->columnsTable->removeRow(selRow);
    ui->columnsTable->setCurrentItem(0);
}

void TableDialog::on_columnUpButton_clicked()
{
    int selRow = ui->columnsTable->currentRow();
    m_model->swapColumns(selRow, selRow - 1);
    swapRowsInTableWidget(selRow, selRow - 1);
}

void TableDialog::on_columnDownButton_clicked()
{
    int selRow = ui->columnsTable->currentRow();
    m_model->swapColumns(selRow, selRow + 1);
    swapRowsInTableWidget(selRow, selRow + 1);
}

void TableDialog::clearColumnsTable()
{
    ui->columnsTable->clear();
    for (int i = ui->columnsTable->rowCount() - 1; i >= 0; --i)
    {
        ui->columnsTable->removeRow(i);
    }
    QStringList labelsHeader;
    labelsHeader << tr("PK") << tr("Name") << tr("Type") << tr("Not null") << tr("Unique");
    ui->columnsTable->setHorizontalHeaderLabels(labelsHeader);
    ui->columnsTable->horizontalHeader()->setHighlightSections(false);
    ui->columnsTable->horizontalHeader()->setClickable(false);
    ui->columnsTable->horizontalHeader()->resizeSection(0, 35);
    ui->columnsTable->horizontalHeader()->resizeSection(1, 200);
    ui->columnsTable->horizontalHeader()->resizeSection(2, 100);
    ui->columnsTable->horizontalHeader()->resizeSection(3, 50);
    ui->columnsTable->horizontalHeader()->resizeSection(4, 50);
}

void TableDialog::applyColumnToTableUI(int row, PColumnModel column)
{
    if (!column)
        return;

    ui->columnsTable->item(row, 0)->setText((column->isConstraintType(Constraint::CT_PrimaryKey) ? "PK" : "" )); // PK
    ui->columnsTable->item(row, 1)->setText(column->name());
    ui->columnsTable->item(row, 2)->setText(column->dataType()->fullTypeName(column->dataTypeParameters()));
    ui->columnsTable->item(row, 3)->setText((column->isConstraintType(Constraint::CT_NotNull) ? "Yes" : "No" )); // Not Null
    ui->columnsTable->item(row, 4)->setText((column->isConstraintType(Constraint::CT_Unique) ? "Yes" : "No" )); // Unique
}

void TableDialog::on_columnsTable_itemSelectionChanged()
{
    int selRow = ui->columnsTable->currentRow();
    ui->columnDownButton->setEnabled(selRow != ui->columnsTable->rowCount() - 1);
    ui->columnUpButton->setEnabled(selRow != 0);
}

void TableDialog::swapRowsInTableWidget(int row1, int row2)
{
    if (row1 == row2)
        return;
}
