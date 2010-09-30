#include <QtCore/QScopedPointer>

#include "columndialog.h"
#include "tabledialog.h"
#include "models/column.h"

#include "ui_tabledialog.h"

TableDialog::TableDialog(QWidget * parent, PSqlDesignerProject project) :
            QDialog(parent),
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
    QScopedPointer<ColumnDialog> dlg(new ColumnDialog(m_model, this));
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
