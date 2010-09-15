#include "columndialog.h"
#include "tabledialog.h"

#include "models/column.h"

#include "ui_tabledialog.h"

TableDialog::TableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableDialog)
{
    ui->setupUi(this);
    m_model = new TableModel(ui->edtTableName->text());
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
    ColumnDialog dlg;
    if (dlg.exec() == QDialog::Accepted)
    {
        PColumnModel c = dlg.newColumn();
        m_model->addColumn(c);
        ui->columnsTable->addItem(c->name());
    }
}

void TableDialog::accept()
{
    m_model->setName(ui->edtTableName->text());
    QDialog::accept();
}

void TableDialog::reject()
{
    delete m_model;
    QDialog::reject();
}
