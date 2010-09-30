#ifndef COLUMNDIALOG_H
#define COLUMNDIALOG_H

#include <QtGui/QDialog>
#include "sqldesignerproject.h"
#include "models/column.h"

namespace Ui {
    class ColumnDialog;
}

class ColumnDialog : public QDialog
{
    Q_OBJECT
public:
    ColumnDialog(PTableModel table = 0, QWidget * parent = 0, PSqlDesignerProject project = 0);
    ~ColumnDialog();
    inline PColumnModel model() const {return m_model;}

    void accept();
    void reject();
protected:
    void changeEvent(QEvent *e);

private:
    PTableModel m_table;
    Ui::ColumnDialog *ui;
    PColumnModel m_model;
    PSqlDesignerProject m_project;

private slots:
    void on_btnAddFK_clicked();
};

#endif // COLUMNDIALOG_H
