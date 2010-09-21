#ifndef FOREIGNKEYSELECTIONVIEWMODEL_H
#define FOREIGNKEYSELECTIONVIEWMODEL_H

#include <QtGui/QStandardItemModel>

class ForeignKeySelectionViewModel : public QStandardItemModel
{
    Q_OBJECT
public:
    ForeignKeySelectionViewModel(QObject * parent = 0);
};

#endif // FOREIGNKEYSELECTIONVIEWMODEL_H
