#ifndef TABLEDIALOG_H
#define TABLEDIALOG_H

#include <QtCore/QString>
#include <QtGui/QDialog>

#include "models/table.h"

namespace Ui {
    class TableDialog;
}

class TableDialog : public QDialog
{
    Q_OBJECT
public:
    TableDialog();
    ~TableDialog();

    inline PTableModel model() const {return m_model;}
    void reject();
    void accept();
protected:
    void changeEvent(QEvent *e);

private:
    Ui::TableDialog *ui;
    PTableModel m_model;

private slots:
    void on_columnAddButton_clicked();
signals:
    void updateTable(QString tableName, PTableModel tablePtr);
    void removeTable(QString tableName);
    void addTable(PTableModel table);
};

#endif // TABLEDIALOG_H
