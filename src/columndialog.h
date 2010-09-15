#ifndef COLUMNDIALOG_H
#define COLUMNDIALOG_H

#include <QDialog>

#include "models/column.h"

namespace Ui {
    class ColumnDialog;
}

class ColumnDialog : public QDialog {
    Q_OBJECT
public:
    ColumnDialog(QWidget *parent = 0);
    ~ColumnDialog();

    PColumnModel newColumn() const;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ColumnDialog *ui;
};

#endif // COLUMNDIALOG_H
