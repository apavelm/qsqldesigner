#ifndef MAGNIFYDIALOG_H
#define MAGNIFYDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class MagnifyDialog;
}

#include "ui_magnifydialog.h"

class MagnifyDialog : public QDialog
{
    Q_OBJECT
public:
    MagnifyDialog(QWidget *parent = 0);
    ~MagnifyDialog();

    inline int value() const { return ui->spinBox->value(); }
    void setValue(int);
protected:
    void changeEvent(QEvent *);

private:
    Ui::MagnifyDialog * ui;
};

#endif // MAGNIFYDIALOG_H
