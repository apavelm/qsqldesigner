#ifndef COLUMNDIALOG_H
#define COLUMNDIALOG_H

#include <QDialog>

namespace Ui {
    class ColumnDialog;
}

class ColumnDialog : public QDialog {
    Q_OBJECT
public:
    ColumnDialog(QWidget *parent = 0);
    ~ColumnDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ColumnDialog *ui;
};

#endif // COLUMNDIALOG_H
