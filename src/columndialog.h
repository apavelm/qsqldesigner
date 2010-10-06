/***************************************************************************
 *   Copyright (C) 2010 by Pavel Andreev                                   *
 *   Mail: apavelm on gmail point com (apavelm@gmail.com)                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, you can read it                      *
 *   here <http://www.gnu.org/licenses/>, or write to the                  *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef COLUMNDIALOG_H
#define COLUMNDIALOG_H

#include <QtGui/QDialog>
#include "models/column.h"
#include "models/datatypes.h"

namespace Ui {
    class ColumnDialog;
}

class ColumnDialog : public QDialog
{
    Q_OBJECT
public:
    ColumnDialog(PColumnModel column, QWidget * parent = 0);
    ~ColumnDialog();

    void accept();
protected:
    void changeEvent(QEvent *e);

private:
    Ui::ColumnDialog *ui;
    PColumnModel m_model;
    DataTypes m_dataTypes;
    QString m_oldDataType;

    void applyDataTypeToUI(PDataType datatype);
    bool hasNoForeignKeyConstraint() const;
    void setUIbuttonFKEnabled(bool hasFK);
    void addFKtoWidget(PConstraint cn);
    void clearFKtable();
private slots:
    void on_btnDelFK_clicked();
    void on_btnAddFK_clicked();
    void changedDataType(const QString& newDataType);
};

#endif // COLUMNDIALOG_H
