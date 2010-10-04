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

#ifndef COLUMNPREVIEWWIDGET_H
#define COLUMNPREVIEWWIDGET_H

#include <QtCore/QModelIndex>
#include <QtGui/QWidget>

class SimpleForeignKeySelectionViewModel;

namespace Ui {
    class ColumnPreviewWidget;
}

class ColumnPreviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ColumnPreviewWidget(QWidget * parent = 0, SimpleForeignKeySelectionViewModel *model = 0);
    ~ColumnPreviewWidget();

    void clearContents();
private:
    Q_DISABLE_COPY(ColumnPreviewWidget)
    SimpleForeignKeySelectionViewModel * m_model;
    Ui::ColumnPreviewWidget *ui;

public slots:
    void onUpdatePreviewWidget(QModelIndex index);
};

#endif // COLUMNPREVIEWWIDGET_H
