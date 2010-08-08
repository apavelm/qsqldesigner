#ifndef TABLEDIALOG_H
#define TABLEDIALOG_H

#include <QDialog>

namespace Ui {
    class TableDialog;
}

class TableDialog : public QDialog {
    Q_OBJECT
public:
    TableDialog(QWidget *parent = 0);
    ~TableDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::TableDialog *ui;
};

#endif // TABLEDIALOG_H
