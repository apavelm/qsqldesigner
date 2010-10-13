#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->lblProgramName->setText(QString("%1\t\t%2").arg(qApp->applicationName()).arg(qApp->applicationVersion()) );
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
