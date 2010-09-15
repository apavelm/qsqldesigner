#include "columndialog.h"
#include "ui_columndialog.h"

ColumnDialog::ColumnDialog(QWidget *parent) :  QDialog(parent),  ui(new Ui::ColumnDialog)
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

PColumnModel ColumnDialog::newColumn() const
{
    PColumnModel c = new ColumnModel();

    c->setName(ui->edtName->text().trimmed());
    c->setComment(ui->edtComment->text().trimmed());
    //c->dataType

    if (!ui->edtDefault->text().trimmed().isEmpty())
    {
        c->addConstraint(new ColumnConstraint("", ColumnConstraint::CT_Default, ui->edtDefault->text().trimmed()));
    }
    if (!ui->edtCheck->text().trimmed().isEmpty())
    {
        c->addConstraint(new ColumnConstraint("", ColumnConstraint::CT_Check, ui->edtCheck->text().trimmed()));
    }
    if (ui->chkPrimaryKey->checkState() && Qt::Checked)
    {
        c->addConstraint(new ColumnConstraint("", ColumnConstraint::CT_PrimaryKey));
    }
    if (ui->chkNotNull->checkState() && Qt::Checked)
    {
        c->addConstraint(new ColumnConstraint("", ColumnConstraint::CT_NotNull));
    }
    if (ui->chkUnique->checkState() && Qt::Checked)
    {
        c->addConstraint(new ColumnConstraint("", ColumnConstraint::CT_Unique));
    }
    // is ForeignKey

    return c;
}
