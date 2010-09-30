#include "foreignkeyselectdialog.h"

#include "models/table.h"
#include "widgets/columnpreviewwidget.h"

#include "ui_foreignkeyselectdialog.h"

ForeignKeySelectDialog::ForeignKeySelectDialog(PColumnModel column, QWidget * parent) : QDialog(parent), ui(new Ui::ForeignKeySelectDialog), m_column(column)
{
    ui->setupUi(this);

    m_model = new ForeignKeySelectionViewModel();
    ui->columnView->setModel(m_model);
    ColumnPreviewWidget * previewWidget = new ColumnPreviewWidget(ui->columnView, m_model);
    connect(ui->columnView, SIGNAL(updatePreviewWidget(QModelIndex)), previewWidget, SLOT(onUpdatePreviewWidget(QModelIndex)));
    ui->columnView->setPreviewWidget(previewWidget);
}

ForeignKeySelectDialog::~ForeignKeySelectDialog()
{
    delete m_model;
    delete ui;
}

void ForeignKeySelectDialog::accept()
{
    QString tableRef, columnRef;
    const QModelIndexList& idxList =  ui->columnView->selectionModel()->selectedIndexes();

    foreach (const QModelIndex& idx, idxList)
    {
        QStandardItem * item = m_model->itemFromIndex(idx);
        QStandardItem * parent = item->parent();
        tableRef = parent->text();
        columnRef = item->text();
    }

    QVariant variant;
    // TODO: FIXME: replace creation of simple FK.
    variant.setValue(ConstraintForeignKey(tableRef, m_column->name(), columnRef));
    m_column->addConstraint(new Constraint(m_column, Constraint::CT_ForeignKey, variant));
    QDialog::accept();
}
