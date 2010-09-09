#ifndef TABLEDIALOG_H
#define TABLEDIALOG_H

#include <QDialog>
#include <QList>

#include "models/table.h"

namespace Ui {
    class TableDialog;
}

class TableDialog : public QDialog
{
    Q_OBJECT
public:
    TableDialog(QWidget *parent = 0);
    ~TableDialog();

    void reject();
    void accept();

    inline TableModel * table() const {return m_model;}

protected:
    void changeEvent(QEvent *e);

private:
    Ui::TableDialog *ui;
    TableModel * m_model;

private slots:
    void on_columnAddButton_clicked();
};

#endif // TABLEDIALOG_H
