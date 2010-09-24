#include "arrowforeignkey.h"
#include "../models/column.h"
#include "../models/modelmanager.h"
#include "widgetmanager.h"
#include "../models/table.h"

#include <math.h>

const qreal Pi = 3.1415;

ArrowForeignKey::ArrowForeignKey(PConstraint constraint, QGraphicsItem *parent, QGraphicsScene *scene) : QGraphicsPathItem(parent, scene), m_initiated(false), m_sourceTable(0), m_refTable(0)
{
    if (constraint)
    {
        m_constraint = constraint;
        if (constraint->type() == Constraint::CT_ForeignKey)
        {
            m_sourceTable = WM->getTableWidgetByName(constraint->column()->table()->name());
            QVariant var;
            if (var.canConvert<ConstraintForeignKey>())
            {
                ConstraintForeignKey fk = var.value<ConstraintForeignKey>();
                m_refTable = WM->getTableWidgetByName(fk.referenceTable());
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

PColumnModel ArrowForeignKey::refColumn() const
{
    QVariant var;
    if (var.canConvert<ConstraintForeignKey>())
    {
        ConstraintForeignKey fk = var.value<ConstraintForeignKey>();
        return MM->getColumnByName(fk.referenceTable(), fk.referenceColumn());
    }
    return 0;
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
