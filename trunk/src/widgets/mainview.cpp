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

#include "mainview.h"
#include "../settingsmanager.h"
#include <cmath>

MainView::MainView(QWidget * parent) : QGraphicsView(parent)
{
    m_gridPoints.clear();
    setDragMode(QGraphicsView::RubberBandDrag);
    setRenderHintsAccordingSettings();
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setCacheMode(QGraphicsView::CacheBackground);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    m_cornerButton.reset( new CornerZoomWidget() );
    m_cornerButton->setPixmap(QPixmap(":/zoom"));
    m_cornerButton->setPixmapSize(verticalScrollBar()->sizeHint().width());
    connect(m_cornerButton.data(), SIGNAL(clicked()), this, SIGNAL(cornerWidgetClicked()));
    setCornerWidget(m_cornerButton.data());
}

MainView::~MainView()
{
}

void MainView::setRenderHintsAccordingSettings()
{
    QPainter::RenderHints flags(0);
    if (SM->antialiasing())
    {
        flags |= QPainter::Antialiasing;
    }
    if (SM->textAntialiasing())
    {
        flags |= QPainter::TextAntialiasing;
    }
    if (SM->smoothPixmapTransform())
    {
        flags |= QPainter::SmoothPixmapTransform;
    }
    if (SM->highQualityAntialiasing())
    {
        flags |= QPainter::HighQualityAntialiasing;
    }
    if (SM->nonCosmeticDefaultPen())
    {
        flags |= QPainter::NonCosmeticDefaultPen;
    }
    setRenderHints(flags);
}

void MainView::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->save();
    // draw background
    QLinearGradient backgroundGradient(rect.left(),rect.top(),rect.left(),rect.bottom());
    backgroundGradient.setColorAt(0, SM->paperColor());
    backgroundGradient.setColorAt(1, QColor(Qt::white));
    painter->setPen(QPen(Qt::NoPen));
    painter->setBrush(QBrush(backgroundGradient));
    painter->drawRect(rect);
    //draw grid
    if (SM->showGrid())
    {
        // don't draw grid, if scale less then 0.5
        if (curScale() > 0.5)
        {
            if (m_gridPoints.isEmpty())
            {
                recalcGridPoints(rect);
            }
            painter->setPen(QPen(Qt::black));
            painter->drawPoints(QPolygonF(m_gridPoints));
        }
    }
    painter->restore();
}

void MainView::recalcGridPoints(const QRectF &rect)
{
    int gridStep = qMin(curScale() * SM->gridSize(), SM->gridSize());

    const qreal MaxX = rect.width();
    const qreal MaxY = rect.height();
    for (qreal y = 0; y <= MaxY; y += gridStep)
    {
        for (qreal x = 0; x <= MaxX; x += gridStep)
        {
            m_gridPoints << QPointF(rect.left() + x, rect.top() + y);
        }
    }
}

void MainView::scaleBy(qreal scaleFactor)
{
    qreal curScaleFactor = transform().m11();
    if (((curScaleFactor == minScale) && (scaleFactor < 1.0)) ||
        ((curScaleFactor == maxScale) && (scaleFactor > 1.0))) return;

    qreal sc = scaleFactor;

    if ((curScaleFactor * sc < minScale)&&(sc < 1.0))
    {
        sc = minScale / curScaleFactor;
    }
    else
        if ((curScaleFactor * sc > maxScale)&&(sc > 1.0))
        {
        sc = maxScale / curScaleFactor;
    }
    m_gridPoints.clear();
    scale(sc, sc);
}

void MainView::setZoom(int percentZoom)
{
    qreal targetScale = (qreal)percentZoom / 100.0;
    qreal scaleFactor = targetScale / transform().m11();
    scaleBy(scaleFactor);
}

void MainView::scrollContentsBy(int dx, int dy)
{
    resetCachedContent();
    QGraphicsView::scrollContentsBy(dx, dy);
}

void MainView::wheelEvent(QWheelEvent *event)
{
   if (event->modifiers() == Qt::ControlModifier)
    {
        scaleBy(std::pow(4.0 / 3.0, (-event->delta() / 240.0)));
    }
    else
    {
        QGraphicsView::wheelEvent(event);
    }
}

void MainView::zoomIn()
{
    scaleBy(1.1);
}

void MainView::zoomOut()
{
    scaleBy(1.0 / 1.1);
}

void MainView::update()
{
    m_gridPoints.clear();
    resetCachedContent();
    QGraphicsView::update();
}

void MainView::setScene(QGraphicsScene * newScene)
{
    scene()->disconnect();
    connect(newScene, SIGNAL(selectionChanged()), this, SLOT(sceneSelectionChanged()));
    QGraphicsView::setScene(newScene);
}

void MainView::sceneSelectionChanged()
{
    emit copyAvailable(scene()->selectedItems().count() != 0);
}
