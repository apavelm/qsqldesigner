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

#include "cornerzoomwidget.h"
#include "mainview.h"

CornerZoomWidget::CornerZoomWidget()
{
    m_pixmap = QPixmap();
    m_size = 32;
    setStatusTip(tr("Click to change Zoom"));
    setMouseTracking(true);
}

void CornerZoomWidget::setPixmap(const QPixmap& pixmap)
{
    m_pixmap = pixmap;
}

void CornerZoomWidget::setPixmapSize(int size)
{
    m_size = size;
}

void CornerZoomWidget::paintEvent(QPaintEvent *)
{
    QStyleOptionButton option;
    option.initFrom(this);
    option.state = QStyle::State_Raised;
    option.icon = QIcon(m_pixmap);
    option.iconSize = QSize(m_size - 4, m_size - 4);
    option.state |= QStyle::State_Enabled;

    QPainter painter(this);
    style()->drawControl(QStyle::CE_PushButton, &option, &painter, this);
}

void CornerZoomWidget::mouseMoveEvent(QMouseEvent *)
{
    MainView * v = qobject_cast<MainView*>(parent());
    int zoomPercent = 100 * v->curScale();
    QString sZoom = tr("Current zoom: %1\%").arg(zoomPercent);
    setToolTip(sZoom);
}

void CornerZoomWidget::mousePressEvent(QMouseEvent *)
{
    emit clicked();
}
