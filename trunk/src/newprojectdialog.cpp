#include "newprojectdialog.h"
#include "ui_newprojectdialog.h"

NewProjectDialog::NewProjectDialog(QWidget *parent) : QDialog(parent), ui(new Ui::NewProjectDialog)
{
    ui->setupUi(this);
}

NewProjectDialog::~NewProjectDialog()
{
    delete ui;
}

const QString NewProjectDialog::projectName() const
{
    return ui->edtProjectName->text();
}

const QString NewProjectDialog::typeDBMS() const
{
    return ui->cbDBMS->currentText();
}
