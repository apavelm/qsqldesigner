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
