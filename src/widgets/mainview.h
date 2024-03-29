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

#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QtCore/QScopedPointer>
#include <QtGui/QGraphicsView>
#include <QtGui/QScrollBar>
#include <QtGui/QToolButton>
#include <QtGui/QWheelEvent>

#include "cornerzoomwidget.h"

class MainView : public QGraphicsView
{
    Q_OBJECT
public:
    static const qreal minScale = 0.1;
    static const qreal maxScale = 5.0;

    explicit MainView(QWidget * parent = 0);
    virtual ~MainView();

    inline qreal curScale() const {return transform().m11();}
    void setRenderHintsAccordingSettings();
public slots:
    void zoomIn();
    void zoomOut();
    void setZoom(int);
    void update();
    void setScene(QGraphicsScene * newScene);

protected:
    void wheelEvent(QWheelEvent * event);

private:
    QScopedPointer<CornerZoomWidget> m_cornerButton;
    QVector<QPointF> m_gridPoints;

    void recalcGridPoints(const QRectF &rect);
    void scaleBy(qreal scaleFactor);
    void drawBackground(QPainter *painter, const QRectF &rect);
    void scrollContentsBy(int dx, int dy);

private slots:
    void sceneSelectionChanged();

signals:
    void cornerWidgetClicked();
    void copyAvailable(bool);
};

#endif // MAINVIEW_H
