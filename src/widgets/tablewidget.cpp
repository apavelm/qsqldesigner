#include "tablewidget.h"
#include "../settingsmanager.h"

const qreal PenWidth = 2.0;
const qreal fColumnPrefixWidth = 25.0;

TableWidget::TableWidget(QGraphicsScene *scene, QGraphicsItem  *parent, TableModel * model) : QGraphicsObject(parent), m_model(model)
{
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges | QGraphicsItem::ItemIsMovable);

    scene->clearSelection();
    scene->addItem(this);
    setSelected(true);
}

QRectF TableWidget::boundingRect() const
{
    QSizeF minSize = recalcMinimumSize();
    return QRectF(-PenWidth/2, -PenWidth/2, minSize.width() + PenWidth, minSize.height() + PenWidth);
}

void TableWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    // declare tools
    QRectF innerBoundingRect = boundingRect().adjusted(PenWidth,0,-PenWidth,0);
    QPen penGray(QColor(127, 128, 127));
    penGray.setStyle(Qt::SolidLine);
    QPen penBlack(penGray);
    penBlack.setStyle(Qt::SolidLine);
    penBlack.setColor(Qt::black);
    QPen penBlackDouble(penBlack);
    penBlackDouble.setWidthF(penBlack.widthF() * 2.0);


    QLinearGradient gradForward(0, 0, boundingRect().width() - PenWidth, 0);
    gradForward.setColorAt(0, SM->tableColorStart());
    gradForward.setColorAt(1, SM->tableColorStop());
    QLinearGradient gradBackward(0, 0, boundingRect().width() - PenWidth, 0);
    gradBackward.setColorAt(0, SM->tableColorStop());
    gradBackward.setColorAt(1, SM->tableColorStart());

    QBrush brushBackGround(gradForward);
    QBrush brushBackward(gradBackward);

    QFont fontNormal(SM->mainFont());
    QFont fontUnderline(fontNormal);
    fontUnderline.setUnderline(true);
    QFont fontBold(fontNormal);
    fontBold.setBold(true);
    QFont smallFont(fontNormal);
    smallFont.setPointSizeF(fontNormal.pointSizeF() * 0.6);
    QFontMetrics metrics(fontNormal);
    QFontMetrics smallMetrics(smallFont);

    const qreal fLineHeight = metrics.boundingRect("WUT").height();
    const qreal fSmallLineHeight = smallMetrics.boundingRect("WUT").height();
    const qreal fLineOffset = fLineHeight / 6;

    qreal fCurrentYPos = 0;

    // yellow block
    painter->setPen(penGray);
    painter->setBrush(brushBackGround);
    painter->drawRect(boundingRect());

    // name block
    painter->setPen(penBlack);
    painter->setFont(fontBold);
    painter->drawText(QRect(0, fCurrentYPos, innerBoundingRect.width(), fLineHeight), Qt::AlignVCenter | Qt::AlignHCenter | Qt::TextSingleLine | Qt::TextDontClip, name());
    fCurrentYPos += fLineHeight + fLineOffset; // set fCurrentYPos to new value

    painter->setPen(penBlackDouble);
    painter->drawLine(innerBoundingRect.left(), fCurrentYPos, innerBoundingRect.width(), fCurrentYPos);
    fCurrentYPos += fLineOffset + penBlackDouble.widthF(); // set fCurrentYPos to new value

    // columns
    if (m_model->columns().count() > 0)
    {
        // draw backward gradient
        painter->setBrush(brushBackward);
        painter->setPen(penGray);
        QRect columnTitleRect(innerBoundingRect.left(), fCurrentYPos, innerBoundingRect.width(), fSmallLineHeight + 2 * PenWidth);
        painter->drawRect(columnTitleRect);

        // printing "column"
        painter->setFont(smallFont);
        painter->drawText(columnTitleRect, Qt::AlignVCenter | Qt::TextSingleLine | Qt::TextDontClip ,"  " + tr("«column»"));

        fCurrentYPos += fSmallLineHeight + 2 * PenWidth + fLineOffset; // set fCurrentYPos to new value
        painter->setPen(QPen(SM->columnFontColor()));
        foreach (const ColumnModel& c, m_model->columns())
        {
            painter->setFont(fontNormal);
            painter->drawText(QRect(innerBoundingRect.left() + PenWidth, fCurrentYPos, fColumnPrefixWidth, fLineHeight), Qt::AlignVCenter, c.getUMLColumnPrefix());

            if (c.isConstraintType(ColumnConstraint::CT_Unique))
            {
                painter->setFont(fontUnderline);
            }

            painter->drawText(QRect(innerBoundingRect.left() + fColumnPrefixWidth, fCurrentYPos, innerBoundingRect.width() - fColumnPrefixWidth, fLineHeight), Qt::AlignVCenter | Qt::TextSingleLine | Qt::TextDontClip, c.getUMLColumnDescription());
            fCurrentYPos += fLineHeight + fLineOffset; // set fCurrentYPos to new value
        }

        int constraintTypeCounters[ColumnConstraint::CT_Last];
        qFill(&constraintTypeCounters[0], &constraintTypeCounters[ColumnConstraint::CT_Last], 0 );
        constraintTypeCounters[ColumnConstraint::CT_PrimaryKey] = m_model->columns().getAmountForType(ColumnConstraint::CT_PrimaryKey);
        constraintTypeCounters[ColumnConstraint::CT_Unique] = m_model->columns().getAmountForType(ColumnConstraint::CT_Unique);
        constraintTypeCounters[ColumnConstraint::CT_ForeignKey] = m_model->columns().getAmountForType(ColumnConstraint::CT_ForeignKey);
        constraintTypeCounters[ColumnConstraint::CT_Default] = m_model->columns().getAmountForType(ColumnConstraint::CT_Default);
        constraintTypeCounters[ColumnConstraint::CT_NotNull] = m_model->columns().getAmountForType(ColumnConstraint::CT_NotNull);
        constraintTypeCounters[ColumnConstraint::CT_Check] = m_model->columns().getAmountForType(ColumnConstraint::CT_Check);

        bool bHasConstraints = false;
        for (int i = 0; i < ColumnConstraint::CT_Last; i++)
        {
            if (constraintTypeCounters[i] > 0)
            {
                bHasConstraints = true;
                break;
            }
        }

        if (bHasConstraints)
        {
            painter->setPen(penBlackDouble);
            painter->drawLine(innerBoundingRect.left(), fCurrentYPos, innerBoundingRect.width(), fCurrentYPos);
            fCurrentYPos += fLineOffset + penBlackDouble.widthF(); // set fCurrentYPos to new value
        }

        // Primary keys section
        if (constraintTypeCounters[ColumnConstraint::CT_PrimaryKey] > 0)
        {
            // draw backward gradient
            painter->setBrush(brushBackward);
            painter->setPen(penGray);
            QRect columnTitleRect(innerBoundingRect.left(), fCurrentYPos, innerBoundingRect.width(), fSmallLineHeight + 2 * PenWidth);
            painter->drawRect(columnTitleRect);
            // printing "PK"
            painter->setFont(smallFont);
            painter->drawText(columnTitleRect, Qt::AlignVCenter | Qt::TextSingleLine | Qt::TextDontClip ,"  " + tr("«PK»"));
            fCurrentYPos += fSmallLineHeight + 2 * PenWidth + fLineOffset; // set fCurrentYPos to new value
            QList<ColumnModel> lstPrimaryKeys;
            m_model->columns().getColumnsForConstraintType(ColumnConstraint::CT_PrimaryKey, lstPrimaryKeys);
            foreach (const ColumnModel& c, lstPrimaryKeys)
            {
                ColumnConstraint cn;
                c.constraints().constraint(ColumnConstraint::CT_PrimaryKey, cn);
                if (cn.type() != ColumnConstraint::CT_Unknown)
                {
                    QString sConstraintName = cn.getUMLConstraintString();
                    painter->setFont(fontNormal);
                    painter->setPen(penBlack);
                    painter->drawText(QRect(innerBoundingRect.left() + PenWidth, fCurrentYPos, fColumnPrefixWidth, fLineHeight), Qt::AlignVCenter, "+");
                    painter->drawText(QRect(innerBoundingRect.left() + fColumnPrefixWidth, fCurrentYPos, innerBoundingRect.width() - fColumnPrefixWidth, fLineHeight), Qt::AlignVCenter | Qt::TextSingleLine | Qt::TextDontClip, sConstraintName);
                    fCurrentYPos += fLineHeight + fLineOffset; // set fCurrentYPos to new value
                }
            }
        }
    }

    // selection outline
    if (isSelected())
        paintSelectionOutline(painter);

    painter->restore();
}

void TableWidget::paintSelectionOutline(QPainter *painter)
{
    QPen pen(painter->pen());
    pen.setStyle(Qt::DashLine);
    pen.setColor(Qt::black);
    pen.setWidth(PenWidth);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(shape());
}

QPainterPath TableWidget::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect().adjusted(PenWidth/2, PenWidth/2, -PenWidth/2, -PenWidth/2));
    return path;
}

void TableWidget::setName(const QString& name)
{
    m_model->setName(name);
}

QSizeF TableWidget::recalcMinimumSize() const
{
    qreal m_minWidth = 50.0;
    qreal m_minHeight = 20.0;

    // calculate table name width
    QFont font(SM->mainFont());
    font.setBold(true);
    QFontMetrics metrics(font);
    const qreal fLineHeight = metrics.boundingRect("WUT").height();
    const qreal fSmallLineHeight = fLineHeight * 0.6;
    const qreal fLineOffset = fLineHeight / 6;
    m_minWidth  = qMax(metrics.boundingRect(name()).width() + (1 + PenWidth) * 4, m_minWidth);
    m_minHeight = qMax(fLineHeight + (1 + PenWidth) * 4, m_minHeight);

    // calculate width depends on columns amount width and height
    if (m_model->columns().count() > 0)
    {
        m_minHeight += fSmallLineHeight + fLineOffset * 2;
        foreach (ColumnModel c, m_model->columns())
        {
            m_minWidth = qMax((qreal)metrics.boundingRect(c.getUMLColumnDescription()).width() + fColumnPrefixWidth, m_minWidth);
            m_minHeight += fLineHeight + fLineOffset;
        }
    }

    int constraintTypeCounters[ColumnConstraint::CT_Last];
    qFill(&constraintTypeCounters[0], &constraintTypeCounters[ColumnConstraint::CT_Last], 0 );
    constraintTypeCounters[ColumnConstraint::CT_PrimaryKey] = m_model->columns().getAmountForType(ColumnConstraint::CT_PrimaryKey);
    constraintTypeCounters[ColumnConstraint::CT_Unique] = m_model->columns().getAmountForType(ColumnConstraint::CT_Unique);
    constraintTypeCounters[ColumnConstraint::CT_ForeignKey] = m_model->columns().getAmountForType(ColumnConstraint::CT_ForeignKey);
    constraintTypeCounters[ColumnConstraint::CT_Default] = m_model->columns().getAmountForType(ColumnConstraint::CT_Default);
    constraintTypeCounters[ColumnConstraint::CT_NotNull] = m_model->columns().getAmountForType(ColumnConstraint::CT_NotNull);
    constraintTypeCounters[ColumnConstraint::CT_Check] = m_model->columns().getAmountForType(ColumnConstraint::CT_Check);

    bool bHasConstraints = false;
    for (int i = 0; i < ColumnConstraint::CT_Last; i++)
    {
        if (constraintTypeCounters[i] > 0)
        {
            bHasConstraints = true;
            break;
        }
    }

    if (bHasConstraints)
    {
        m_minHeight += fSmallLineHeight + fLineOffset * 2;
    }

    // calculate width depends on constraints amount width and height
    if (constraintTypeCounters[ColumnConstraint::CT_PrimaryKey] > 0)
    {
        QList<ColumnModel> lstPrimaryKeys;
        m_model->columns().getColumnsForConstraintType(ColumnConstraint::CT_PrimaryKey, lstPrimaryKeys);
        foreach (const ColumnModel& c, lstPrimaryKeys)
        {
            ColumnConstraint cn;
            c.constraints().constraint(ColumnConstraint::CT_PrimaryKey, cn);
            if (cn.type() != ColumnConstraint::CT_Unknown)
            {
                QString sConstraintName = cn.getUMLConstraintString();
                m_minWidth = qMax((qreal)metrics.boundingRect(sConstraintName).width() + fColumnPrefixWidth, m_minWidth);
                m_minHeight += fLineHeight + fLineOffset;
            }
        }
    }

    return QSizeF(m_minWidth, m_minHeight);
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
