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

TableDialog::TableDialog(QWidget * parent, PSqlDesignerProject project) :
            QDialog(parent),
            m_project(project),
            ui(new Ui::TableDialog)
{
    ui->setupUi(this);
    m_model = new TableModel(project->modelManager());
    ui->edtTableName->setText(m_model->name());

    connect(this, SIGNAL(addTable(PTableModel)), project->modelManager(), SLOT(addTable(PTableModel)));
    connect(this, SIGNAL(removeTable(QString)), project->modelManager(), SLOT(removeTable(QString)));
    connect(this, SIGNAL(updateTable(QString,PTableModel)), project->modelManager(), SIGNAL(tableUpdate(QString,PTableModel)));
//    emit addTable(m_model);
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
    ColumnDialog dlg(m_model, this, m_project);
    if (dlg.exec() == QDialog::Accepted)
    {
        ui->columnsTable->addItem(dlg.model()->name());
    }
}

void TableDialog::accept()
{
    m_model->setName(ui->edtTableName->text());
    emit addTable(m_model);
    QDialog::accept();
}

void TableDialog::reject()
{
    emit removeTable(m_model->name());
    QDialog::reject();
}
