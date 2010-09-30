#ifndef FOREIGNKEYSELECTDIALOG_H
#define FOREIGNKEYSELECTDIALOG_H

#include <QtGui/QDialog>

#include "sqldesignerproject.h"
#include "models/column.h"
#include "foreignkeyselectionviewmodel.h"

namespace Ui {
    class ForeignKeySelectDialog;
}

class ForeignKeySelectDialog : public QDialog
{
    Q_OBJECT

public:
    ForeignKeySelectDialog(PColumnModel column, QWidget * parent = 0, PSqlDesignerProject project = 0);
    ~ForeignKeySelectDialog();

    void accept();
private:
    Ui::ForeignKeySelectDialog *ui;
    ForeignKeySelectionViewModel * m_model;
    PColumnModel m_column;
    PSqlDesignerProject m_project;
};

#endif // FOREIGNKEYSELECTDIALOG_H
