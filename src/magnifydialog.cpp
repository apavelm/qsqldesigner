#include "magnifydialog.h"

MagnifyDialog::MagnifyDialog(QWidget *parent) : QDialog(parent), ui(new Ui::MagnifyDialog)
{
    ui->setupUi(this);
}

MagnifyDialog::~MagnifyDialog()
{
    delete ui;
}

void MagnifyDialog::changeEvent(QEvent *e)
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

void MagnifyDialog::setValue(int val)
{
    ui->spinBox->setValue(val);
}
