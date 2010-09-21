#ifndef COLUMNPREVIEWWIDGET_H
#define COLUMNPREVIEWWIDGET_H

#include <QtCore/QModelIndex>
#include <QtGui/QWidget>

class ForeignKeySelectionViewModel;

namespace Ui {
    class ColumnPreviewWidget;
}

class ColumnPreviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ColumnPreviewWidget(QWidget * parent = 0, ForeignKeySelectionViewModel *model = 0);
    ~ColumnPreviewWidget();

    void clearContents();
private:
    Q_DISABLE_COPY(ColumnPreviewWidget)
    ForeignKeySelectionViewModel * m_model;
    Ui::ColumnPreviewWidget *ui;

public slots:
    void onUpdatePreviewWidget(QModelIndex index);
};

#endif // COLUMNPREVIEWWIDGET_H
