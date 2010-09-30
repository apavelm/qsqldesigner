#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include <QtCore/QString>
#include <QtGui/QDialog>

namespace Ui {
    class NewProjectDialog;
}

class NewProjectDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NewProjectDialog(QWidget *parent = 0);
    ~NewProjectDialog();

    const QString projectName() const;
    const QString typeDBMS() const;
private:
    Ui::NewProjectDialog *ui;
};

#endif // NEWPROJECTDIALOG_H
