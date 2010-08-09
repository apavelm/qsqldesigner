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
    QSize minSize(50, 20);

    // calculate table name width
    QPainter painter;
    QFont font(painter.font());
    font.setBold(true);
    painter.setFont(font);
    QFontMetrics metrics(font);
    minSize.setWidth(qMax(metrics.boundingRect(name()).width() + 8, minSize.width()));
    minSize.setHeight(qMax(metrics.boundingRect(name()).height() + 8, minSize.height()));

    // calculate children items width
    foreach (QGraphicsItem* item, childItems())
    {
        minSize.setWidth(qMax(static_cast<int>(item->boundingRect().width()), minSize.width()));
        minSize.setHeight(minSize.height() + item->boundingRect().height());
    }

    return QRectF(QPoint(0, 0), minSize);
}

void TableWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    QPen pen(QColor(127, 128, 127));
    pen.setStyle(Qt::SolidLine);
    QBrush brush(QColor(255, 0xfc, 0xcc), Qt::SolidPattern);
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(boundingRect());

    QString sName = name();
    painter->setPen(Qt::black);
    QFont font(painter->font());
    font.setBold(true);
    painter->setFont(font);
    QFontMetrics metrics(font);
    painter->drawText(QRect(4, 4, metrics.boundingRect(sName).width() + 4, metrics.boundingRect(sName).height() + 4), sName);

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

QDataStream &operator<<(QDataStream &out, const TableWidget &item)
{
    /*out << smileyItem.pos() << smileyItem.angle()
        << smileyItem.shearHorizontal() << smileyItem.shearVertical()
        << smileyItem.zValue()
        << static_cast<qint16>(smileyItem.face())
        << smileyItem.pen() << smileyItem.brush()
        << smileyItem.isShowingHat();
    */return out;
}


QDataStream &operator>>(QDataStream &in, TableWidget &item)
{
    /*QPointF position;
    double angle;
    double shearHorizontal;
    double shearVertical;
    double z;
    qint16 face;
    QPen pen;
    QBrush brush;
    bool showHat;
    in >> position >> angle >> shearHorizontal >> shearVertical >> z
       >> face >> pen >> brush >> showHat;
    smileyItem.setPos(position);
    smileyItem.setAngle(angle);
    smileyItem.setShear(shearHorizontal, shearVertical);
    smileyItem.setZValue(z);
    smileyItem.setFace(static_cast<SmileyItem::Face>(face));
    smileyItem.setPen(pen);
    smileyItem.setBrush(brush);
    smileyItem.setShowHat(showHat);*/
    return in;
}
