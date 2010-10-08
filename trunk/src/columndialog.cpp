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
#include "ui_columndialog.h"

#include "pluginmanager.h"
#include "sqldesignerprojectsettings.h"

#include <QtGui/QMessageBox>

ColumnDialog::ColumnDialog(PColumnModel column, QWidget * parent) :
            QDialog(parent),
            ui(new Ui::ColumnDialog),
            m_model(column)
{
    ui->setupUi(this);
    clearFKtable();

    ui->edtName->setText(m_model->name());
    m_dataTypes = PLUGINMANAGER->dataTypesForDatabase(m_model->table()->modelManager()->projectSettings()->dbmsType());
    ui->cbDataType->addItems(m_dataTypes.toStringList());
    m_oldDataType = (column->dataType() == 0 ? ui->cbDataType->currentText() : column->dataType()->typeName());

    ui->edtSize->setValue(m_model->dataTypeParameters().first);
    ui->edtPrecision->setValue(m_model->dataTypeParameters().second);

    ui->edtComment->setText(m_model->comment());

    PConstraint cn = 0;
    cn = m_model->constraint(Constraint::CT_Default);
    if (cn)
    {
        QVariant var = cn->data();
        if (var.canConvert<QString>())
        {
            QString sDefault = var.value<QString>();
            ui->edtDefault->setText(sDefault);
        }
    }

    cn = m_model->constraint(Constraint::CT_Check);
    if (cn)
    {
        QVariant var = cn->data();
        if (var.canConvert<QString>())
        {
            QString sCheck = var.value<QString>();
            ui->edtCheck->setText(sCheck);
        }
    }

    ui->chkPrimaryKey->setCheckState( (m_model->constraint(Constraint::CT_PrimaryKey) == 0 ? Qt::Unchecked : Qt::Checked ) );
    ui->chkNotNull->setCheckState( (m_model->constraint(Constraint::CT_NotNull) == 0 ? Qt::Unchecked : Qt::Checked ) );
    ui->chkUnique->setCheckState( (m_model->constraint(Constraint::CT_Unique) == 0 ? Qt::Unchecked : Qt::Checked ) );

    PConstraint pFK = m_model->constraint(Constraint::CT_ForeignKey);
    addFKtoWidget(pFK);
    setUIbuttonFKEnabled(pFK == 0);

    connect(ui->cbDataType, SIGNAL(currentIndexChanged(QString)), this, SLOT(changedDataType(QString)));
    applyDataTypeToUI(m_dataTypes.typeByName(m_oldDataType));
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
    PDataType pDataType = m_dataTypes.typeByName(ui->cbDataType->currentText());
    m_model->setDataType(pDataType);

    if (pDataType->parametersAmount() > 0)
    {
        int a1 = ui->edtSize->value();
        int a2 = (pDataType->parametersAmount() > 1 ?  ui->edtPrecision->value() : 0);
        m_model->setDataTypeParameters(qMakePair<int, int>(a1, a2));
    }

    // Primary key
    if (ui->chkPrimaryKey->checkState() && Qt::Checked)
    {
        if (!m_model->isConstraintType(Constraint::CT_PrimaryKey))
        {
            m_model->addConstraint(new Constraint(m_model, Constraint::CT_PrimaryKey));
        }
    }
    else
    {
        if (m_model->isConstraintType(Constraint::CT_PrimaryKey))
        {
            m_model->deleteConstraint(Constraint::CT_PrimaryKey);
        }
    }

    // Not null
    if (ui->chkNotNull->checkState() && Qt::Checked)
    {
        if (!m_model->isConstraintType(Constraint::CT_NotNull))
        {
            m_model->addConstraint(new Constraint(m_model, Constraint::CT_NotNull));
        }
    }
    else
    {
        if (m_model->isConstraintType(Constraint::CT_NotNull))
        {
            m_model->deleteConstraint(Constraint::CT_NotNull);
        }
    }


    // Unique
    if (ui->chkUnique->checkState() && Qt::Checked)
    {
        if (!m_model->isConstraintType(Constraint::CT_Unique))
        {
            m_model->addConstraint(new Constraint(m_model, Constraint::CT_Unique));
        }
    }
    else
    {
        if (m_model->isConstraintType(Constraint::CT_Unique))
        {
            m_model->deleteConstraint(Constraint::CT_Unique);
        }
    }

    // Default
    if (m_model->isConstraintType(Constraint::CT_Default))
    {
        m_model->deleteConstraint(Constraint::CT_Default);
        if (!ui->edtDefault->text().trimmed().isEmpty())
        {
            m_model->addConstraint(new Constraint(m_model, Constraint::CT_Default, ui->edtDefault->text().trimmed()));
        }
    }
    else
    {
        if (!ui->edtDefault->text().trimmed().isEmpty())
        {
            m_model->addConstraint(new Constraint(m_model, Constraint::CT_Default, ui->edtDefault->text().trimmed()));
        }
    }

    // Check
    if (m_model->isConstraintType(Constraint::CT_Check))
    {
        m_model->deleteConstraint(Constraint::CT_Check);
        if (!ui->edtCheck->text().trimmed().isEmpty())
        {
            m_model->addConstraint(new Constraint(m_model, Constraint::CT_Check, ui->edtCheck->text().trimmed()));
        }
    }
    else
    {
        if (!ui->edtCheck->text().trimmed().isEmpty())
        {
            m_model->addConstraint(new Constraint(m_model, Constraint::CT_Check, ui->edtCheck->text().trimmed()));
        }
    }

    QDialog::accept();
}

void ColumnDialog::addFKtoWidget(PConstraint cn)
{
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
                QTableWidgetItem *item2 = new QTableWidgetItem(m_model->dataType()->fullTypeName(m_model->dataTypeParameters()));
                item2->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
                ui->viewForeignKeys->setItem(row, 2, item2);

                setUIbuttonFKEnabled(true);
            }
        }
    }
}

void ColumnDialog::on_btnAddFK_clicked()
{
    m_model->setName(ui->edtName->text().trimmed());
    m_model->setDataType(m_dataTypes.typeByName(ui->cbDataType->currentText()));

    SimpleForeignKeySelectDialog dlg(m_model, this);
    if (dlg.exec() == QDialog::Accepted)
    {
        addFKtoWidget(dlg.constraint());
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
                ui->viewForeignKeys->removeRow(0);
                applyDataTypeToUI(m_dataTypes.typeByName(newDataType));
                setUIbuttonFKEnabled(false);
                return;
            }
            else
            {
                // NO
                int idx = ui->cbDataType->findText(m_oldDataType, Qt::MatchFixedString);
                if (idx != -1)
                {
                    ui->cbDataType->setCurrentIndex(idx);
                }
                return;
            }
        }
        m_oldDataType = newDataType;
        applyDataTypeToUI(m_dataTypes.typeByName(newDataType));
    }
    else
    {
        if (!m_oldDataType.isEmpty())
        {
            int idx = ui->cbDataType->findText(m_oldDataType, Qt::MatchFixedString);
            if (idx != -1)
            {
                ui->cbDataType->setCurrentIndex(idx);
            }
        }
    }
}

void ColumnDialog::applyDataTypeToUI(PDataType datatype)
{
    ui->cbDataType->setCurrentIndex(ui->cbDataType->findText(datatype->typeName(), Qt::MatchFixedString));
    ui->edtPrecision->setEnabled(datatype->parametersAmount() == 2);
    ui->blPrecision->setEnabled(datatype->parametersAmount() == 2);

    ui->edtSize->setEnabled(datatype->parametersAmount() > 0);
    ui->lblSize->setEnabled(datatype->parametersAmount() > 0);
    setUIbuttonFKEnabled(!hasNoForeignKeyConstraint());
}


bool ColumnDialog::hasNoForeignKeyConstraint() const
{
    return !m_model->isConstraintType(Constraint::CT_ForeignKey);
}

void ColumnDialog::setUIbuttonFKEnabled(bool hasFK)
{
    ui->btnAddFK->setEnabled(!hasFK);
    ui->btnDelFK->setEnabled(hasFK);
}

void ColumnDialog::on_btnDelFK_clicked()
{
    m_model->deleteConstraint(Constraint::CT_ForeignKey);
    clearFKtable();
}

void ColumnDialog::clearFKtable()
{
    ui->viewForeignKeys->clear();
    for (int i = ui->viewForeignKeys->rowCount() - 1; i >= 0; --i)
    {
        ui->viewForeignKeys->removeRow(i);
    }
    QStringList labelsHeader;
    labelsHeader << tr("Table") << tr("Column") << tr("Data Type");
    ui->viewForeignKeys->setHorizontalHeaderLabels(labelsHeader);
    ui->viewForeignKeys->horizontalHeader()->setHighlightSections(false);
    ui->viewForeignKeys->horizontalHeader()->setClickable(false);
    ui->viewForeignKeys->horizontalHeader()->resizeSection(0, 100);
    ui->viewForeignKeys->horizontalHeader()->resizeSection(1, 200);
    ui->viewForeignKeys->horizontalHeader()->resizeSection(2, 87);
    setUIbuttonFKEnabled(false);
}
