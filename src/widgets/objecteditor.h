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

#ifndef OBJECTEDITOR_H
#define OBJECTEDITOR_H

#include <QtCore/QScopedPointer>
#include <QtGui/QWidget>

#include "../sqldesignerproject.h"
#include "widgetviewmodel.h"

namespace Ui {
    class ObjectEditor;
}

class ObjectEditor : public QWidget
{
    Q_OBJECT
public:
    explicit ObjectEditor(QWidget *parent = 0);
    ~ObjectEditor();

    void setProject(PSqlDesignerProject project);

private:
    Ui::ObjectEditor *ui;
    PSqlDesignerProject m_project;
    QScopedPointer<WidgetViewModel> m_browserModel;
public slots:
    void updateModel();
};

#endif // OBJECTEDITOR_H
