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

#include <QtCore/QScopedPointer>
#include <QtGui/QDialog>

#include "models/column.h"
#include "simpleforeignkeyselectionviewmodel.h"

namespace Ui {
    class SimpleForeignKeySelectDialog;
}

class SimpleForeignKeySelectDialog : public QDialog
{
    Q_OBJECT
public:
    SimpleForeignKeySelectDialog(PColumnModel column, QWidget * parent = 0);
    ~SimpleForeignKeySelectDialog();

    void accept();
    inline PConstraint constraint() const {return m_constraint;}
private:
    Ui::SimpleForeignKeySelectDialog *ui;
    QScopedPointer<SimpleForeignKeySelectionViewModel> m_model;
    PColumnModel m_column;
    PConstraint m_constraint;
};

#endif // FOREIGNKEYSELECTDIALOG_H
