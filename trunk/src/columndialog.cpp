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
#include "simpleforeignkeyselectdialog.h"
#include "models/table.h"
#include "ui_columndialog.h"

#include "pluginmanager.h"
#include "projectmanager.h"

#include <QtGui/QMessageBox>

ColumnDialog::ColumnDialog(PTableModel table, QWidget * parent, PSqlDesignerProject project) :  QDialog(parent),  m_table(table), ui(new Ui::ColumnDialog)
{
    ui->setupUi(this);
    m_project = project;
    m_model = new ColumnModel(table);
    ui->edtName->setText(m_model->name());
    m_dataTypes = PLUGINMANAGER->dataTypesForDatabase(CURRENTPROJECT->dbmsType());

    ui->cbDataType->addItems(m_dataTypes.toStringList());
    m_oldDataType = ui->cbDataType->currentText();
    connect(ui->cbDataType, SIGNAL(currentIndexChanged(QString)), this, SLOT(changedDataType(QString)));
}

ColumnDialog::~ColumnDialog()
{
    delete ui;
}

void ColumnDialog::changeEvent(QEvent *e)
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

void ColumnDialog::accept()
{
    m_model->setName(ui->edtName->text().trimmed());
    m_model->setComment(ui->edtComment->text().trimmed());
    m_model->setDataType(m_dataTypes.typeByName(ui->cbDataType->currentText()));

    if (!ui->edtDefault->text().trimmed().isEmpty())
    {
        m_model->addConstraint(new Constraint(m_model, Constraint::CT_Default, ui->edtDefault->text().trimmed()));
    }
    if (!ui->edtCheck->text().trimmed().isEmpty())
    {
        m_model->addConstraint(new Constraint(m_model, Constraint::CT_Check, ui->edtCheck->text().trimmed()));
    }
    if (ui->chkPrimaryKey->checkState() && Qt::Checked)
    {
        m_model->addConstraint(new Constraint(m_model, Constraint::CT_PrimaryKey));
    }
    if (ui->chkNotNull->checkState() && Qt::Checked)
    {
        m_model->addConstraint(new Constraint(m_model, Constraint::CT_NotNull));
    }
    if (ui->chkUnique->checkState() && Qt::Checked)
    {
        m_model->addConstraint(new Constraint(m_model, Constraint::CT_Unique));
    }

    m_table->addColumn(m_model);
    QDialog::accept();
}

void ColumnDialog::reject()
{
    if (m_model)
    {
        delete m_model;
    }
    QDialog::reject();
}

void ColumnDialog::on_btnAddFK_clicked()
{
    m_model->setName(ui->edtName->text().trimmed());
    m_model->setDataType(m_dataTypes.typeByName(ui->cbDataType->currentText()));

    SimpleForeignKeySelectDialog dlg(m_model, this, m_project);
    if (dlg.exec() == QDialog::Accepted)
    {
        PConstraint cn = dlg.constraint();
        if (cn)
        {
            if (cn->type() == Constraint::CT_ForeignKey)
            {
                // adding foreign key inforamtion to TableWidget
                QVariant var = cn->data();
                if (var.canConvert<ConstraintForeignKey>())
                {
                    ConstraintForeignKey m_fk = var.value<ConstraintForeignKey>();

                    int row = ui->viewForeignKeys->rowCount();
                    ui->viewForeignKeys->insertRow(row);
                    QTableWidgetItem *item0 = new QTableWidgetItem(QIcon(":/table24"), m_fk.referenceTable());
                    ui->viewForeignKeys->setItem(row, 0, item0);
                    QTableWidgetItem *item1 = new QTableWidgetItem(QIcon(":/column24"), m_fk.referenceColumns().at(0));
                    ui->viewForeignKeys->setItem(row, 1, item1);
                    QTableWidgetItem *item2 = new QTableWidgetItem(m_model->dataType().typeName);
                    item2->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
                    ui->viewForeignKeys->setItem(row, 2, item2);
                }
            }
        }
    }
}

void ColumnDialog::changedDataType(const QString& newDataType)
{
    if (!newDataType.isEmpty())
    {
        if (m_model->isConstraintType(Constraint::CT_ForeignKey))
        {
            if (QMessageBox::question(this, tr("Changing data type"), tr("Are you sure to changecolumn datatype now?\nThis column has FOREIGN KEY constraint, and it will be deleted if you change this column type.\n\nAre you sure?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
            {
                // YES
                m_model->deleteConstraint(Constraint::CT_ForeignKey);
                ui->viewForeignKeys->clearContents();
                applyDataTypeToUI(m_dataTypes.typeByName(newDataType));
                // TODO: insert setEnable(true/false) for "add FK" button
            }
            else
            {
                // NO
                int idx = ui->cbDataType->findText(m_oldDataType, Qt::MatchFixedString);
                if (idx != -1)
                    ui->cbDataType->setCurrentIndex(idx);
                return;
            }
        }
    }
    m_oldDataType = newDataType;
}

void ColumnDialog::applyDataTypeToUI(const DataType& datatype)
{
    ui->edtPrecision->setEnabled(datatype.parametersAmount == 2);
    ui->blPrecision->setEnabled(datatype.parametersAmount == 2);

    ui->edtSize->setEnabled(datatype.parametersAmount > 0);
    ui->lblSize->setEnabled(datatype.parametersAmount > 0);
}
