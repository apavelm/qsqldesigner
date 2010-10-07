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
    ui->edtTableName->setText(m_model->name());

    ui->columnsTable->updateColumnListModel(m_model->columns());

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
        m_model->addColumn(column);
        ui->columnsTable->appendRow(column);
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
    int row = ui->columnsTable->currentIndex().row();
    PColumnModel column = m_model->column(row);
    if (column)
    {
        ColumnDialog dlg(column, this);
        if (dlg.exec() == QDialog::Accepted)
        {
            ui->columnsTable->updateRow(row, column);
        }
    }
}

void TableDialog::on_columnDeleteButton_clicked()
{
    int row = ui->columnsTable->currentIndex().row();
    if (ui->columnsTable->removeRow(row))
    {
        m_model->removeColumn(row);
        ui->columnsTable->setCurrentIndex(QModelIndex());
    }
}

void TableDialog::on_columnUpButton_clicked()
{
    int selRow = ui->columnsTable->currentIndex().row();
    if (ui->columnsTable->swapRows(selRow, selRow - 1))
    {
        m_model->swapColumns(selRow, selRow - 1);
    }
}

void TableDialog::on_columnDownButton_clicked()
{
    int selRow = ui->columnsTable->currentIndex().row();
    if (ui->columnsTable->swapRows(selRow, selRow + 1))
    {
        m_model->swapColumns(selRow, selRow + 1);
    }
}

void TableDialog::on_columnsTable_selectedRowSignal(int row)
{
    if (row == -1)
    {
        // disable all except "add"
        ui->columnDeleteButton->setEnabled(false);
        ui->columnEditButton->setEnabled(false);
        ui->columnDownButton->setEnabled(false);
        ui->columnUpButton->setEnabled(false);
    }
    else
        if (row == 0)
        {
            // enable all except "up"
            ui->columnDeleteButton->setEnabled(true);
            ui->columnEditButton->setEnabled(true);
            ui->columnDownButton->setEnabled(true);
            ui->columnUpButton->setEnabled(false);
        }
        else
            if (row == m_model->columns().count() - 1)
            {
                // enable all except "down"
                ui->columnDeleteButton->setEnabled(true);
                ui->columnEditButton->setEnabled(true);
                ui->columnDownButton->setEnabled(false);
                ui->columnUpButton->setEnabled(true);
            }
            else
                {
                    // enable all
                    ui->columnDeleteButton->setEnabled(true);
                    ui->columnEditButton->setEnabled(true);
                    ui->columnDownButton->setEnabled(true);
                    ui->columnUpButton->setEnabled(true);
                }
}
