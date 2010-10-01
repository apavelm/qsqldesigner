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

#ifndef FOREIGNKEYSELECTDIALOG_H
#define FOREIGNKEYSELECTDIALOG_H

#include <QtGui/QDialog>

#include "sqldesignerproject.h"
#include "models/column.h"
#include "foreignkeyselectionviewmodel.h"

namespace Ui {
    class ForeignKeySelectDialog;
}

class ForeignKeySelectDialog : public QDialog
{
    Q_OBJECT

public:
    ForeignKeySelectDialog(PColumnModel column, QWidget * parent = 0, PSqlDesignerProject project = 0);
    ~ForeignKeySelectDialog();

    void accept();
private:
    Ui::ForeignKeySelectDialog *ui;
    ForeignKeySelectionViewModel * m_model;
    PColumnModel m_column;
    PSqlDesignerProject m_project;
};

#endif // FOREIGNKEYSELECTDIALOG_H
