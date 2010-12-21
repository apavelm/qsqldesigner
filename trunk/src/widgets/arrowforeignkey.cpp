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

#include "widgetmanager.h"
#include "arrowforeignkey.h"
#include "../models/column.h"
#include "../models/modelmanager.h"
#include "../models/table.h"
#include "../settingsmanager.h"

#include <math.h>
#include <QGraphicsSceneHoverEvent>

ArrowForeignKey::ArrowForeignKey(PWidgetManager manager, PConstraint constraint) :
                QGraphicsObject(),
                m_wm(manager),
                m_sourceTable(0),
                m_refTable(0),
                m_locStartPos(-1.0, -1.0),
                m_locStopPos(-1.0, -1.0)
{
    Q_ASSERT(constraint);
    m_contextMenu.reset(new QMenu(tr("Foreign Key Menu")));

    m_sourceTable = m_wm->getTableWidgetByName(constraint->tableName());
    QVariant var = constraint->data();
    if (var.canConvert<ConstraintForeignKey>())
    {
        m_fk = var.value<ConstraintForeignKey>();
        m_refTable = m_wm->getTableWidgetByName(m_fk.referenceTable());
    }

    setAcceptHoverEvents(true);

    connect(m_refTable, SIGNAL(xChanged()), this, SLOT(updatePosition()));
    connect(m_refTable, SIGNAL(yChanged()), this, SLOT(updatePosition()));
    connect(m_sourceTable, SIGNAL(xChanged()), this, SLOT(updatePosition()));
    connect(m_sourceTable, SIGNAL(yChanged()), this, SLOT(updatePosition()));


    updatePosition();
}

ArrowForeignKey::~ArrowForeignKey()
{
}

QRectF ArrowForeignKey::boundingRect() const
{
    qreal extra = SM->penWidth() / 2 + (qreal)SM->arrowSize();
    return m_path.boundingRect().normalized().adjusted(-extra, -extra, extra, extra);
}

void ArrowForeignKey::updatePosition()
{
    if (m_waypoints.isEmpty())
    {
        // new arrow, calculating closest points table1 and table2
        //QPair<QPointF, QPointF> pt = closestRectPoints();
       // m_locStartPos = pt.first;
        //m_locStopPos = pt.second;
    }

    m_startPos = mapFromItem(m_sourceTable, m_locStartPos);
    m_stopPos = mapFromItem(m_refTable, m_locStopPos);

    QPainterPath path;
    path.moveTo(m_startPos);
    path.lineTo(m_stopPos);

    QLineF line(m_stopPos, m_startPos);
    double angle = acos(line.dx() / line.length());
    double Pi = 3.1415;
    int arrowSize = SM->arrowSize();
    if (line.dy() >= 0)
    {
        angle = 2 * Pi  - angle;
    }
    m_arrowHead1 = m_stopPos + QPointF(sin(angle + Pi / 3) * arrowSize, cos(angle + Pi / 3) * arrowSize);
    m_arrowHead2 = m_stopPos + QPointF(sin(angle + Pi - Pi / 3) * arrowSize, cos(angle + Pi - Pi / 3) * arrowSize);

    m_path = path;
}

#include <QtDebug>
void ArrowForeignKey::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << mapToItem(this, event->scenePos());
}

void ArrowForeignKey::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    hasCursor = true;
}

void ArrowForeignKey::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    hasCursor = false;
}

QPainterPath ArrowForeignKey::shape() const
{
    QPainterPath shapePath(m_path);
    QPolygonF arrow;
    arrow << m_stopPos << m_arrowHead1 << m_arrowHead2 << m_stopPos;
    shapePath.addPolygon(arrow);
    return shapePath;
}

void ArrowForeignKey::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();
    painter->setPen(QPen(Qt::black, hasCursor ? SM->penWidth()*2 : SM->penWidth(), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawPath(m_path);
    painter->drawLine(m_stopPos, m_arrowHead1);
    painter->drawLine(m_stopPos, m_arrowHead2);
    painter->restore();
}
