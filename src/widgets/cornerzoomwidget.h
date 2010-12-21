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

#ifndef CORNERZOOMWIDGET_H
#define CORNERZOOMWIDGET_H

#include <QtGui/QStyleOptionButton>
#include <QtGui/QStylePainter>
#include <QtGui/QWidget>

class CornerZoomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CornerZoomWidget(QWidget * parent = 0);
    virtual ~CornerZoomWidget() {}

    void setPixmap(const QPixmap& pixmap);
    void setPixmapSize(int size);
protected:
   void paintEvent(QPaintEvent *);
   void mouseMoveEvent(QMouseEvent *);
   void mousePressEvent(QMouseEvent *);
private:
    QPixmap m_pixmap;
    int m_size;
signals:
    void clicked();
};

#endif // CORNERZOOMWIDGET_H
