#include "tablewidget.h"

TableWidget::TableWidget(QGraphicsScene *scene, QGraphicsItem  *parent, TableModel * model) : QGraphicsObject(parent), m_model(model)
{
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges | QGraphicsItem::ItemIsMovable);

    scene->clearSelection();
    scene->addItem(this);
    setSelected(true);
}

QRectF TableWidget::boundingRect() const
{
    return QRectF(0,0, 100,100);
}

void TableWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    QPen pen(Qt::yellow);
    pen.setStyle(Qt::SolidLine);
    QBrush brush(Qt::blue, Qt::SolidPattern);
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(boundingRect());

    if (isSelected())
        paintSelectionOutline(painter);

    painter->restore();
}

QPainterPath TableWidget::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void TableWidget::paintSelectionOutline(QPainter *painter)
{
    QPen pen(Qt::DashLine);
    pen.setColor(Qt::black);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(shape());
}


void TableWidget::setName(const QString& name)
{
    m_model->setName(name);
}

QVariant TableWidget::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        //foreach (Arrow *arrow, arrows) {
            //arrow->updatePosition();
        //}
    }

    return QGraphicsItem::itemChange(change, value);
}

void TableWidget::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    //myContextMenu->exec(event->screenPos());
}
