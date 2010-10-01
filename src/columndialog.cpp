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
#include "foreignkeyselectdialog.h"
#include "models/table.h"
#include "ui_columndialog.h"

#include "pluginmanager.h"
#include "projectmanager.h"

ColumnDialog::ColumnDialog(PTableModel table, QWidget * parent, PSqlDesignerProject project) :  QDialog(parent),  m_table(table), ui(new Ui::ColumnDialog)
{
    ui->setupUi(this);
    m_project = project;
    m_model = new ColumnModel(table);
    ui->edtName->setText(m_model->name());
    m_dataTypes = PLUGINMANAGER->dataTypesForDatabase(CURRENTPROJECT->dbmsType());

    ui->cbDataType->addItems(m_dataTypes.toStringList());
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
    m_model->setDataType(m_dataTypes.typeByAcronim(ui->cbDataType->currentText()));

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
    // is ForeignKey

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
    ForeignKeySelectDialog dlg(m_model, this, m_project);
    dlg.exec();
}
