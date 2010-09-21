#include "foreignkeyselectionviewmodel.h"
#include "models/modelmanager.h"

ForeignKeySelectionViewModel::ForeignKeySelectionViewModel(QObject *parent) : QStandardItemModel(parent)
{
    QList<QString> lstTables = MM->getTableList();
    foreach (const QString& tableName, lstTables)
    {
        // Create the phone groups as QStandardItems
        QStandardItem * table = new QStandardItem(QIcon(":/table24"), tableName);

        QList<QString> lstColumns = MM->getColumnList(tableName);
        foreach (const QString& columnName, lstColumns)
        {
            QStandardItem * column = new QStandardItem(QIcon(":/column24"), columnName);
            // the appendRow function appends the column as new row
            table->appendRow(column);
        }
        // append table as new row to the model. model takes the ownership of the item
        appendRow(table);
    }
}

