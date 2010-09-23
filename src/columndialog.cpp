#include <QtCore/QScopedPointer>

#include "columndialog.h"
#include "foreignkeyselectdialog.h"
#include "models/table.h"
#include "ui_columndialog.h"

ColumnDialog::ColumnDialog(PTableModel table) :  QDialog(0),  m_table(table), ui(new Ui::ColumnDialog)
{
    ui->setupUi(this);
    m_model = new ColumnModel(table);
    ui->edtName->setText(m_model->name());
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
    //m_model->dataType

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
    QScopedPointer<ForeignKeySelectDialog> dlg( new ForeignKeySelectDialog(m_model));
    dlg->exec();
}
