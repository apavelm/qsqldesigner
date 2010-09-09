#include "columndialog.h"
#include "ui_columndialog.h"

ColumnDialog::ColumnDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColumnDialog)
{
    ui->setupUi(this);
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

ColumnModel ColumnDialog::newColumn() const
{
    ColumnModel c;
    c.columnName = ui->edtName->text();
    c.defaultValue = ui->edtDefault->text();
    //c.dataType
    c.columnComment = ui->txtComment->toPlainText();
    c.isNotNull = ui->chkNotNull->checkState() && Qt::Checked;
    c.isPrimaryKey = ui->chkPrimaryKey->checkState() && Qt::Checked;
    c.isUnique = ui->chkUnique->checkState() && Qt::Checked;
    return c;
}
