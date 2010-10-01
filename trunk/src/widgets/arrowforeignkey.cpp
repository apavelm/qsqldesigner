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

ArrowForeignKey::ArrowForeignKey(PWidgetManager manager, PConstraint constraint) : QGraphicsPathItem(), m_wm(manager), m_initiated(false), m_sourceTable(0), m_refTable(0)
{
    if (constraint)
    {
        m_constraint = constraint;
        if (constraint->type() == Constraint::CT_ForeignKey)
        {
            m_sourceTable = m_wm->getTableWidgetByName(constraint->column()->table()->name());
            QVariant var = m_constraint->data();
            if (var.canConvert<ConstraintForeignKey>())
            {
                m_fk = var.value<ConstraintForeignKey>();
                m_refTable = m_wm->getTableWidgetByName(m_fk.referenceTable());
            }
            setFlag(QGraphicsItem::ItemIsSelectable, true);
            setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

            if (m_refTable && m_sourceTable)
            {
                m_initiated = true;
            }
        }
    }
}

ArrowForeignKey::~ArrowForeignKey()
{
    //m_constraint->column()->deleteConstraint(m_constraint);
}

QRectF ArrowForeignKey::boundingRect() const
{
    if (m_initiated)
    {
        return path().boundingRect();
    }
    else
        return QRectF();
}

QPainterPath ArrowForeignKey::shape() const
{
    if (m_initiated)
    {
        QPainterPath path = QGraphicsPathItem::shape();
        //path.addPolygon(m_arrowHead);
        return path;
    }
    else
        return QPainterPath();
}

void ArrowForeignKey::updatePosition()
{
    if (m_initiated)
    {
        QLineF line(mapFromItem(m_sourceTable, 0, 0), mapFromItem(m_refTable, 0, 0));
        QPainterPath path;
        path.moveTo(line.p1());
        path.lineTo(line.p2());
        setPath(path);
    }
}

void ArrowForeignKey::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (m_initiated)
    {
        painter->save();
        painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawPath(path());
        painter->restore();
    }
}
