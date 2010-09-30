#include "columnpreviewwidget.h"
#include "../foreignkeyselectionviewmodel.h"
#include "../models/modelmanager.h"
#include "../projectmanager.h"

#include "ui_columnpreviewwidget.h"

ColumnPreviewWidget::ColumnPreviewWidget(QWidget *parent, ForeignKeySelectionViewModel *model) : QWidget(parent), m_model(model), ui(new Ui::ColumnPreviewWidget)
{
    ui->setupUi(this);
}

ColumnPreviewWidget::~ColumnPreviewWidget()
{
    delete ui;
}

void ColumnPreviewWidget::onUpdatePreviewWidget(QModelIndex index)
{
    if (m_model)
    {
        QStandardItem * item = m_model->itemFromIndex(index);
        if (item)
        {
            QStandardItem * parent = item->parent();
            if (parent)
            {
                const QString tableName = parent->text();
                const QString columnName = item->text();

                PColumnModel column = CURRENTPROJECT->modelManager()->getColumnByName(tableName, columnName);

                ui->lblColumnName->setText(column->name());
                const QString & comment = column->comment();
                if (comment.isEmpty())
                {
                    ui->labelComment->hide();
                }
                else
                {
                    ui->labelComment->show();
                }
                ui->lblColumnComment->setText(comment);
                ui->lblColumnDatatype->setText(""); // TODO: insert DATATYPE serialization string
             }
        }
    }
    else
        clearContents();
}

void ColumnPreviewWidget::clearContents()
{
    ui->lblColumnName->clear();
    ui->lblColumnComment->clear();
    ui->lblColumnDatatype->clear();
}
