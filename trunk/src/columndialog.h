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
#include "sqldesignerproject.h"
#include "models/column.h"
#include "models/datatypes.h"

namespace Ui {
    class ColumnDialog;
}

class ColumnDialog : public QDialog
{
    Q_OBJECT
public:
    ColumnDialog(PTableModel table = 0, QWidget * parent = 0, PSqlDesignerProject project = 0);
    ~ColumnDialog();
    inline PColumnModel model() const {return m_model;}

    void accept();
    void reject();
protected:
    void changeEvent(QEvent *e);

private:
    PTableModel m_table;
    Ui::ColumnDialog *ui;
    PColumnModel m_model;
    PSqlDesignerProject m_project;
    DataTypes m_dataTypes;


private slots:
    void on_btnAddFK_clicked();
};

#endif // COLUMNDIALOG_H
