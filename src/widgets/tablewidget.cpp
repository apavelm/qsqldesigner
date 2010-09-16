#include "tablewidget.h"
#include "../settingsmanager.h"

TableWidget::TableWidget(QGraphicsScene *scene, QGraphicsItem  *parent, TableModel * model) : QGraphicsObject(parent), m_model(model), m_name(model->name())
{
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges | QGraphicsItem::ItemIsMovable);

    scene->clearSelection();
    scene->addItem(this);
    setSelected(true);
}

QRectF TableWidget::boundingRect() const
{
    QSizeF minSize = recalcMinimumSize();
    const qreal PenWidth = SM->penWidth();
    return QRectF(-PenWidth/2, -PenWidth/2, minSize.width() + PenWidth, minSize.height() + PenWidth);
}

void TableWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    const qreal PenWidth = SM->penWidth();

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
    QFont smallFont(SM->smallFont());
    QFont fontUnderline(fontNormal);
    fontUnderline.setUnderline(true);
    QFont fontBold(fontNormal);
    fontBold.setBold(true);
    QFontMetrics metrics(fontNormal);
    QFontMetrics smallMetrics(smallFont);

    const qreal fLineHeight = metrics.lineSpacing();
    const qreal fSmallLineHeight = smallMetrics.lineSpacing();
    const qreal fLineOffset = fLineHeight / 5;
    const qreal fColumnPrefixWidth = 1.05 * metrics.boundingRect("*PFK").width();

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
        fCurrentYPos += fLineOffset;
        painter->setBrush(brushBackward);
        painter->setPen(penGray);
        QRect columnTitleRect(innerBoundingRect.left(), fCurrentYPos, innerBoundingRect.width(), fSmallLineHeight + 2 * PenWidth);
        painter->drawRect(columnTitleRect);

        // printing "column"
        painter->setFont(smallFont);
        painter->drawText(columnTitleRect, Qt::AlignVCenter | Qt::TextSingleLine | Qt::TextDontClip ,"  " + tr("첽olumn�"));

        fCurrentYPos += fSmallLineHeight + 2 * PenWidth + fLineOffset; // set fCurrentYPos to new value
        painter->setPen(QPen(SM->columnFontColor()));
        foreach (SharedColumnModel c, m_model->columns()) // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        {
            painter->setFont(fontNormal);
            painter->drawText(QRect(innerBoundingRect.left() + PenWidth, fCurrentYPos, fColumnPrefixWidth, fLineHeight), Qt::AlignVCenter, c->getUMLColumnPrefix());

            if (c->isConstraintType(ColumnConstraint::CT_Unique))
            {
                painter->setFont(fontUnderline);
            }

            painter->drawText(QRect(innerBoundingRect.left() + fColumnPrefixWidth, fCurrentYPos, innerBoundingRect.width() - fColumnPrefixWidth, fLineHeight), Qt::AlignVCenter | Qt::TextSingleLine | Qt::TextDontClip, c->getUMLColumnDescription());
            fCurrentYPos += fLineHeight; // set fCurrentYPos to new value
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
            fCurrentYPos += fLineOffset;
            painter->setPen(penBlackDouble);
            painter->drawLine(innerBoundingRect.left(), fCurrentYPos, innerBoundingRect.width(), fCurrentYPos);
            fCurrentYPos += fLineOffset + penBlackDouble.widthF(); // set fCurrentYPos to new value
        }

        // Foreign keys section
        if (constraintTypeCounters[ColumnConstraint::CT_ForeignKey] > 0)
        {
            // draw backward gradient
            fCurrentYPos += fLineOffset;
            painter->setBrush(brushBackward);
            painter->setPen(penGray);
            QRect columnTitleRect(innerBoundingRect.left(), fCurrentYPos, innerBoundingRect.width(), fSmallLineHeight + 2 * PenWidth);
            painter->drawRect(columnTitleRect);
            // printing "FK"
            painter->setFont(smallFont);
            painter->drawText(columnTitleRect, Qt::AlignVCenter | Qt::TextSingleLine | Qt::TextDontClip ,"  " + tr("첛K�"));
            fCurrentYPos += fSmallLineHeight + 2 * PenWidth + fLineOffset; // set fCurrentYPos to new value

            QList<PColumnModel> lstForeignKeys;
            m_model->columns().getColumnsForConstraintType(ColumnConstraint::CT_ForeignKey, lstForeignKeys);
            foreach (PColumnModel c, lstForeignKeys)
            {
                PColumnConstraint cn = c->constraints().constraint(ColumnConstraint::CT_ForeignKey);
                if (cn->type() != ColumnConstraint::CT_Unknown)
                {
                    QString sConstraintName = cn->getUMLConstraintString();
                    painter->setFont(fontNormal);
                    painter->setPen(penBlack);
                    painter->drawText(QRect(innerBoundingRect.left() + PenWidth, fCurrentYPos, fColumnPrefixWidth, fLineHeight), Qt::AlignVCenter, "+");
                    painter->drawText(QRect(innerBoundingRect.left() + fColumnPrefixWidth, fCurrentYPos, innerBoundingRect.width() - fColumnPrefixWidth, fLineHeight), Qt::AlignVCenter | Qt::TextSingleLine | Qt::TextDontClip, sConstraintName);
                    fCurrentYPos += fLineHeight; // set fCurrentYPos to new value
                }
            }
        }

        // Primary keys section
        if (constraintTypeCounters[ColumnConstraint::CT_PrimaryKey] > 0)
        {
            // draw backward gradient
            fCurrentYPos += fLineOffset;
            painter->setBrush(brushBackward);
            painter->setPen(penGray);
            QRect columnTitleRect(innerBoundingRect.left(), fCurrentYPos, innerBoundingRect.width(), fSmallLineHeight + 2 * PenWidth);
            painter->drawRect(columnTitleRect);
            // printing "PK"
            painter->setFont(smallFont);
            painter->drawText(columnTitleRect, Qt::AlignVCenter | Qt::TextSingleLine | Qt::TextDontClip ,"  " + tr("첧K�"));
            fCurrentYPos += fSmallLineHeight + 2 * PenWidth + fLineOffset; // set fCurrentYPos to new value

            QList<PColumnModel> lstPrimaryKeys;
            m_model->columns().getColumnsForConstraintType(ColumnConstraint::CT_PrimaryKey, lstPrimaryKeys);
            foreach (PColumnModel c, lstPrimaryKeys)
            {
                PColumnConstraint cn = c->constraints().constraint(ColumnConstraint::CT_PrimaryKey);
                if (cn->type() != ColumnConstraint::CT_Unknown)
                {
                    QString sConstraintName = cn->getUMLConstraintString();
                    painter->setFont(fontNormal);
                    painter->setPen(penBlack);
                    painter->drawText(QRect(innerBoundingRect.left() + PenWidth, fCurrentYPos, fColumnPrefixWidth, fLineHeight), Qt::AlignVCenter, "+");
                    painter->drawText(QRect(innerBoundingRect.left() + fColumnPrefixWidth, fCurrentYPos, innerBoundingRect.width() - fColumnPrefixWidth, fLineHeight), Qt::AlignVCenter | Qt::TextSingleLine | Qt::TextDontClip, sConstraintName);
                    fCurrentYPos += fLineHeight; // set fCurrentYPos to new value
                }
            }
        }
        // Unique section
        if (constraintTypeCounters[ColumnConstraint::CT_Unique] > 0)
        {
            // draw backward gradient
            fCurrentYPos += fLineOffset;
            painter->setBrush(brushBackward);
            painter->setPen(penGray);
            QRect columnTitleRect(innerBoundingRect.left(), fCurrentYPos, innerBoundingRect.width(), fSmallLineHeight + 2 * PenWidth);
            painter->drawRect(columnTitleRect);
            // printing "unique"
            painter->setFont(smallFont);
            painter->drawText(columnTitleRect, Qt::AlignVCenter | Qt::TextSingleLine | Qt::TextDontClip ,"  " + tr("쳕nique�"));
            fCurrentYPos += fSmallLineHeight + 2 * PenWidth + fLineOffset; // set fCurrentYPos to new value

            QList<PColumnModel> lstUnique;
            m_model->columns().getColumnsForConstraintType(ColumnConstraint::CT_Unique, lstUnique);
            foreach (PColumnModel c, lstUnique)
            {
                PColumnConstraint cn = c->constraints().constraint(ColumnConstraint::CT_Unique);
                if (cn->type() != ColumnConstraint::CT_Unknown)
                {
                    QString sConstraintName = cn->getUMLConstraintString();
                    painter->setFont(fontNormal);
                    painter->setPen(penBlack);
                    painter->drawText(QRect(innerBoundingRect.left() + PenWidth, fCurrentYPos, fColumnPrefixWidth, fLineHeight), Qt::AlignVCenter, "+");
                    painter->drawText(QRect(innerBoundingRect.left() + fColumnPrefixWidth, fCurrentYPos, innerBoundingRect.width() - fColumnPrefixWidth, fLineHeight), Qt::AlignVCenter | Qt::TextSingleLine | Qt::TextDontClip, sConstraintName);
                    fCurrentYPos += fLineHeight; // set fCurrentYPos to new value
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
    pen.setWidth(SM->penWidth());
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(shape());
}

QPainterPath TableWidget::shape() const
{
    QPainterPath path;
    const qreal PenWidth = SM->penWidth();
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
    QFont fontNormal(SM->mainFont());
    QFont fontBold(fontNormal);
    fontBold.setBold(true);
    QFont smallFont(SM->smallFont());
    QFontMetrics metrics(fontNormal);
    QFontMetrics boldMetrics(fontBold);
    QFontMetrics smallMetrics(smallFont);
    const qreal fLineHeight = metrics.lineSpacing();
    const qreal fSmallLineHeight = smallMetrics.lineSpacing();
    const qreal fLineOffset = fLineHeight / 5;
    const qreal fColumnPrefixWidth = 1.05 * metrics.boundingRect("*PFK").width();
    const qreal PenWidth = SM->penWidth();

    m_minWidth  = qMax(boldMetrics.boundingRect(name()).width() + (1 + PenWidth) * 4, m_minWidth);
    m_minHeight = qMax(fLineHeight + (1 + PenWidth) * 4, m_minHeight);

    const int nColumnAmount = m_model->columns().count();

    int constraintTypeCounters[ColumnConstraint::CT_Last];
    qFill(&constraintTypeCounters[0], &constraintTypeCounters[ColumnConstraint::CT_Last], 0 );

    if (nColumnAmount > 0)
    {
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

        // calculate height
        m_minHeight += fSmallLineHeight + fLineOffset * 2; // for separator line
        m_minHeight += nColumnAmount * fLineHeight; // height per columns * amount of columns
        if (bHasConstraints)
            m_minHeight += fSmallLineHeight + fLineOffset * 2; // for separator line, if there are any constraints
        if (constraintTypeCounters[ColumnConstraint::CT_ForeignKey] > 0)
        {
            m_minHeight += fSmallLineHeight + 2 * PenWidth + fLineOffset;
            m_minHeight += (qreal)constraintTypeCounters[ColumnConstraint::CT_ForeignKey] * fLineHeight;
        }
        if (constraintTypeCounters[ColumnConstraint::CT_PrimaryKey] > 0)
        {
            m_minHeight += fSmallLineHeight + 2 * PenWidth + fLineOffset;
            m_minHeight += (qreal)constraintTypeCounters[ColumnConstraint::CT_PrimaryKey] * fLineHeight;
        }
        if (constraintTypeCounters[ColumnConstraint::CT_Unique] > 0)
        {
            m_minHeight += fSmallLineHeight + 2 * PenWidth + fLineOffset;
            m_minHeight += (qreal)constraintTypeCounters[ColumnConstraint::CT_Unique] * fLineHeight;
        }

        // calculate width
        m_minWidth = qMax(m_model->longestStringWidth(metrics) + 2 * fColumnPrefixWidth, m_minWidth);
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
