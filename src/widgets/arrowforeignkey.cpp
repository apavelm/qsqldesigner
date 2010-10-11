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

ArrowForeignKey::ArrowForeignKey(PWidgetManager manager, PConstraint constraint) :
                QObject(manager),
                QGraphicsPathItem(),
                m_wm(manager),
                m_sourceTable(0),
                m_refTable(0),
                m_locStartPos(-1.0, -1.0),
                m_locStopPos(-1.0, -1.0)
{
    if (constraint)
    {
        m_sourceTable = m_wm->getTableWidgetByName(constraint->column()->table()->name());
        QVariant var = constraint->data();
        if (var.canConvert<ConstraintForeignKey>())
        {
            m_fk = var.value<ConstraintForeignKey>();
            m_refTable = m_wm->getTableWidgetByName(m_fk.referenceTable());
        }
        setPen(QPen(Qt::black, SM->penWidth(), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        //setFlag(QGraphicsItem::ItemIsSelectable, true);
        connect(m_refTable, SIGNAL(xChanged()), this, SLOT(updatePosition()));
        connect(m_refTable, SIGNAL(yChanged()), this, SLOT(updatePosition()));
        connect(m_sourceTable, SIGNAL(xChanged()), this, SLOT(updatePosition()));
        connect(m_sourceTable, SIGNAL(yChanged()), this, SLOT(updatePosition()));
        updatePosition();
    }
}

ArrowForeignKey::~ArrowForeignKey()
{
}

QRectF ArrowForeignKey::boundingRect() const
{
    qreal extra = SM->penWidth() / 2 + (qreal)SM->arrowSize();
    return path().boundingRect().normalized().adjusted(-extra, -extra, extra, extra);
}

QPair<QPointF, QPointF> ArrowForeignKey::closestRectPoints() const
{
    // getting coord-point of rectangles
    QPointF points[4];
    points[0] = mapFromItem(m_sourceTable, QPointF(0, 0));
    QPointF sz1(m_sourceTable->boundingRect().size().width(), m_sourceTable->boundingRect().size().height());
    points[1] = mapFromItem(m_sourceTable, sz1);
    points[2] = mapFromItem(m_refTable, QPointF(0, 0));
    QPointF sz2(m_refTable->boundingRect().size().width(), m_refTable->boundingRect().size().height());
    points[3] = mapFromItem(m_refTable, sz2);

    QPointF dMatrix[4][4];
    for (int i = 0; i< 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            dMatrix[i][j] = points[i] - points[j];
        }
    }

    int score[2][4];
    // 0 - left, 1 - top, 2 - right, 3 - bottom
    for (int i = 0; i< 4; i++)
    {
        score[0][i] = 0;
        score[1][i] = 0;
    }

        if (dMatrix[3][1].x() > 0)
        {
            score[1][0]+=2; // score to left edge of 2nd
            score[0][2]+=2; // score to right edge of 1st
        }
        else
        {
            score[0][0]++; // score to left edge of 1st
            score[1][2]++; // score to right edge of 2nd
        }
        if (dMatrix[3][1].x() < 0)
        {
            score[0][0]+=2; // score to left edge of 1st
            score[1][2]+=2; // score to right edge of 2nd
        }
        else
        {
            score[1][0]++; // score to left edge of 2nd
            score[0][2]++; // score to right edge of 1st
        }

        if (dMatrix[3][1].y() > 0)
        {
            score[0][3]+=2; // score to bottom edge of 1st
            score[1][1]+=2; // score to top edge of 2nd
        }
        else
        {
            score[1][3]++; // score to bottom edge of 2nd
            score[0][1]++; // score to top edge of 1st
        }
        if (dMatrix[3][1].y() < 0)
        {
            score[1][3]+=2; // score to bottom edge of 2nd
            score[0][1]+=2; // score to top edge of 1st
        }
        else
        {
            score[0][3]++; // score to bottom edge of 1st
            score[1][1]++; // score to top edge of 2nd
        }

        if (dMatrix[2][1].x() > 0)
        {
            score[1][0]+=2; // score to left edge of 2nd
            score[0][2]+=2; // score to right edge of 1st
        }
        else
        {
            score[0][0]++; // score to left edge of 1st
            score[1][2]++; // score to right edge of 2nd
        }
        if (dMatrix[2][1].x() < 0)
        {
            score[0][0]+=2; // score to left edge of 1st
            score[1][2]+=2; // score to right edge of 2nd
        }
        else
        {
            score[1][0]++; // score to left edge of 2nd
            score[0][2]++; // score to right edge of 1st
        }

        if (dMatrix[2][1].y() > 0)
        {
            score[1][1]+=2; // score to top edge of 2nd
            score[0][3]+=2; // score to bottom edge of 1st
        }
        else
        {
            score[0][1]++; // score to top edge of 1st
            score[1][3]++; // score to bottom edge of 2nd
        }
        if (dMatrix[2][1].y() < 0)
        {
            score[0][1]+=2; // score to top edge of 1st
            score[1][3]+=2; // score to bottom edge of 2nd
        }
        else
        {
            score[1][1]++; // score to top edge of 2nd
            score[0][3]++; // score to bottom edge of 1st
        }

        if (dMatrix[2][0].x() > 0)
        {
            score[1][0]+=2; // score to left edge of 2nd
            score[0][2]+=2; // score to right edge of 1st
        }
        else
        {
            score[0][0]++; // score to left edge of 1st
            score[1][2]++; // score to right edge of 2nd
        }
        if (dMatrix[2][0].x() < 0)
        {
            score[0][0]+=2; // score to left edge of 1st
            score[1][2]+=2; // score to right edge of 2nd
        }
        else
        {
            score[1][0]++; // score to left edge of 2nd
            score[0][2]++; // score to right edge of 1st
        }

        if (dMatrix[2][0].y() > 0)
        {
            score[1][1]+=2; // score to top edge of 2nd
            score[0][3]+=2; // score to bottom edge of 1st
        }
        else
        {
            score[0][1]++; // score to top edge of 1st
            score[1][3]++; // score to bottom edge of 2nd
        }
        if (dMatrix[2][0].y() < 0)
        {
            score[0][1]+=2; // score to top edge of 1st
            score[1][3]+=2; // score to bottom edge of 2nd
        }
        else
        {
            score[1][1]++; // score to top edge of 2nd
            score[0][3]++; // score to bottom edge of 1st
        }

        if (dMatrix[3][0].x() > 0)
        {
            score[1][0]+=2; // score to left edge of 2nd
            score[0][2]+=2; // score to right edge of 1st
        }
        else
        {
            score[0][0]++; // score to left edge of 1st
            score[1][2]++; // score to right edge of 2nd
        }
        if (dMatrix[3][0].x() < 0)
        {
            score[0][0]+=2; // score to left edge of 1st
            score[1][2]+=2; // score to right edge of 2nd
        }
        else
        {
            score[1][0]++; // score to left edge of 2nd
            score[0][2]++; // score to right edge of 1st
        }

        if (dMatrix[3][0].y() > 0)
        {
            score[1][1]+=2; // score to top edge of 2nd
            score[0][3]+=2; // score to bottom edge of 1st
        }
        else
        {
            score[0][1]++; // score to top edge of 1st
            score[1][3]++; // score to bottom edge of 2nd
        }
        if (dMatrix[3][0].y() < 0)
        {
            score[0][1]+=2; // score to top edge of 1st
            score[1][3]+=2; // score to bottom edge of 2nd
        }
        else
        {
            score[1][1]++; // score to top edge of 2nd
            score[0][3]++; // score to bottom edge of 1st
        }

        // selecting best score edge
        int selectedEdge1 = 0;
        int selectedEdge2 = 0;
        int max1 = 0;
        int max2 = 0;
        // selecting MAX values
        for (int i = 0; i < 4; i++)
        {
            max1 = qMax(score[0][i], max1);
            max2 = qMax(score[1][i], max2);
        }

        // count amount of MAX values
        QSet<int> v1, v2;
        for (int i = 0; i < 4; i++)
        {
            if (score[0][i] == max1)
                v1 << i;
            if (score[1][i] == max2)
                v2 << i;
        }
        int c1 = v1.count();
        int c2 = v2.count();

            if (c1 > 0)
            {
                if (v1.contains(0) && v1.contains(1))
                {
                    // top left
                    if (qAbs(dMatrix[0][2].x()) > qAbs(dMatrix[0][2].y()))
                    {
                        selectedEdge1 = 0;
                    }
                    else
                    {
                        selectedEdge1 = 1;
                    }
                }
                if (v1.contains(1) && v1.contains(2))
                {
                    // top right
                    if (qAbs(dMatrix[1][2].x()) > qAbs(dMatrix[1][2].y()))
                    {
                        selectedEdge1 = 1;
                    }
                    else
                    {
                        selectedEdge1 = 2;
                    }
                }
                if (v1.contains(2) && v1.contains(3))
                {
                    // bottom right
                    if (qAbs(dMatrix[1][3].x()) > qAbs(dMatrix[1][3].y()))
                    {
                        selectedEdge1 = 3;
                    }
                    else
                    {
                        selectedEdge1 = 2;
                    }
                }
                if (v1.contains(3) && v1.contains(0))
                {
                    // bottom left
                    if (qAbs(dMatrix[1][2].x()) > qAbs(dMatrix[2][0].y()))
                    {
                        selectedEdge1 = 3;
                    }
                    else
                    {
                        selectedEdge1 = 0;
                    }
                }

            }
            if (c2 > 0)
            {
                if (v2.contains(0) && v2.contains(1))
                {
                    // top left
                    if (qAbs(dMatrix[0][2].x()) > qAbs(dMatrix[0][2].y()))
                    {
                        selectedEdge2 = 0;
                    }
                    else
                    {
                        selectedEdge2 = 1;
                    }
                }
                if (v2.contains(1) && v2.contains(2))
                {
                    // top right
                    if (qAbs(dMatrix[1][2].x()) > qAbs(dMatrix[1][2].y()))
                    {
                        selectedEdge2 = 1;
                    }
                    else
                    {
                        selectedEdge2 = 2;
                    }
                }
                if (v2.contains(2) && v2.contains(3))
                {
                    // bottom right
                    if (qAbs(dMatrix[1][3].x()) > qAbs(dMatrix[1][3].y()))
                    {
                        selectedEdge2 = 3;
                    }
                    else
                    {
                        selectedEdge2 = 2;
                    }
                }
                if (v2.contains(3) && v2.contains(0))
                {
                    // bottom left
                    if (qAbs(dMatrix[1][2].x()) > qAbs(dMatrix[2][0].y()))
                    {
                        selectedEdge2 = 3;
                    }
                    else
                    {
                        selectedEdge2 = 0;
                    }
                }
            }


        qreal px1, px2, py1, py2;
        switch (selectedEdge1)
        {
            case 0: px1 = 0; py1 = sz1.y() / 2; break;
            case 1: px1 = sz1.x() / 2; py1 = 0; break;
            case 2: px1 = sz1.x(); py1 = sz1.y() / 2; break;
            case 3: px1 = sz1.x() / 2; py1 = sz1.y(); break;
            default: break;
        }
        switch (selectedEdge2)
        {
            case 0: px2 = 0; py2 = sz2.y() / 2; break;
            case 1: px2 = sz2.x() / 2; py2 = 0; break;
            case 2: px2 = sz2.x(); py2 = sz2.y() / 2; break;
            case 3: px2 = sz2.x() / 2; py2 = sz2.y(); break;
            default: break;
        }
        return qMakePair<QPointF, QPointF>(QPointF(px1, py1), QPointF(px2, py2));
}

void ArrowForeignKey::updatePosition()
{
    if (m_waypoints.isEmpty())
    {
        // new arrow, calculating closest points table1 and table2
        QPair<QPointF, QPointF> pt = closestRectPoints();
        m_locStartPos = pt.first;
        m_locStopPos = pt.second;
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

    setPath(path);
}

QPainterPath ArrowForeignKey::shape() const
{
    QPainterPath path = QGraphicsPathItem::shape();
    QPolygonF arrow;
    arrow << m_stopPos << m_arrowHead1 << m_arrowHead2 << m_stopPos;
    path.addPolygon(arrow);
    return path;
}

void ArrowForeignKey::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();
    painter->setPen(QPen(Qt::black, SM->penWidth(), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawPath(path());
    painter->drawLine(m_stopPos, m_arrowHead1);
    painter->drawLine(m_stopPos, m_arrowHead2);
    painter->restore();
}
