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
    explicit TableDialog(PTableModel table, QWidget * parent = 0);
    virtual ~TableDialog();

    void accept();
protected:
    void changeEvent(QEvent *e);

private:
    Ui::TableDialog *ui;
    PTableModel m_model;
private slots:
    void on_columnDownButton_clicked();
    void on_columnUpButton_clicked();
    void on_columnDeleteButton_clicked();
    void on_columnEditButton_clicked();
    void on_columnAddButton_clicked();
    void on_columnsTable_selectedRowSignal(int row);
};

#endif // TABLEDIALOG_H
