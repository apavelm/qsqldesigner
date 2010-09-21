#include <QtCore/QScopedPointer>

#include "columndialog.h"
#include "tabledialog.h"
#include "models/column.h"

#include "ui_tabledialog.h"

TableDialog::TableDialog(QWidget *parent) : QDialog(parent), ui(new Ui::TableDialog)
{
    ui->setupUi(this);
    m_model = new TableModel();
    ui->edtTableName->setText(m_model->name());
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
    QScopedPointer<ColumnDialog> dlg(new ColumnDialog(m_model));
    if (dlg->exec() == QDialog::Accepted)
    {
        ui->columnsTable->addItem(dlg->model()->name());
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
