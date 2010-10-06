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

#include "tablewidget.h"
#include "../settingsmanager.h"

TableWidget::TableWidget(QGraphicsScene *scene, QGraphicsItem  *parent, TableModel * model) : QGraphicsObject(parent), m_model(model), m_name(model->name())
{
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable);

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
        painter->drawText(columnTitleRect, Qt::AlignVCenter | Qt::TextSingleLine | Qt::TextDontClip ,"  " + tr("columns"));

        fCurrentYPos += fSmallLineHeight + 2 * PenWidth + fLineOffset; // set fCurrentYPos to new value
        painter->setPen(QPen(SM->columnFontColor()));
        foreach (const SharedColumnModel& c, m_model->columns()) // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        {
            painter->setFont(fontNormal);
            painter->drawText(QRect(innerBoundingRect.left() + PenWidth, fCurrentYPos, fColumnPrefixWidth, fLineHeight), Qt::AlignVCenter, c->getUMLColumnPrefix());

            if (c->isConstraintType(Constraint::CT_Unique))
            {
                painter->setFont(fontUnderline);
            }

            painter->drawText(QRect(innerBoundingRect.left() + fColumnPrefixWidth, fCurrentYPos, innerBoundingRect.width() - fColumnPrefixWidth, fLineHeight), Qt::AlignVCenter | Qt::TextSingleLine | Qt::TextDontClip, c->getUMLColumnDescription());
            fCurrentYPos += fLineHeight; // set fCurrentYPos to new value
        }

        int constraintTypeCounters[Constraint::CT_Last];
        qFill(&constraintTypeCounters[0], &constraintTypeCounters[Constraint::CT_Last], 0 );
        constraintTypeCounters[Constraint::CT_PrimaryKey] = m_model->columns().getAmountForType(Constraint::CT_PrimaryKey);
        constraintTypeCounters[Constraint::CT_Unique] = m_model->columns().getAmountForType(Constraint::CT_Unique);
        constraintTypeCounters[Constraint::CT_ForeignKey] = m_model->columns().getAmountForType(Constraint::CT_ForeignKey);
        constraintTypeCounters[Constraint::CT_Default] = m_model->columns().getAmountForType(Constraint::CT_Default);
        constraintTypeCounters[Constraint::CT_NotNull] = m_model->columns().getAmountForType(Constraint::CT_NotNull);
        constraintTypeCounters[Constraint::CT_Check] = m_model->columns().getAmountForType(Constraint::CT_Check);

        bool bHasConstraints = false;
        for (int i = 0; i < Constraint::CT_Last; i++)
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
        if (constraintTypeCounters[Constraint::CT_ForeignKey] > 0)
        {
            // draw backward gradient
            fCurrentYPos += fLineOffset;
            painter->setBrush(brushBackward);
            painter->setPen(penGray);
            QRect columnTitleRect(innerBoundingRect.left(), fCurrentYPos, innerBoundingRect.width(), fSmallLineHeight + 2 * PenWidth);
            painter->drawRect(columnTitleRect);
            // printing "FK"
            painter->setFont(smallFont);
            painter->drawText(columnTitleRect, Qt::AlignVCenter | Qt::TextSingleLine | Qt::TextDontClip , "  " + tr("FK"));
            fCurrentYPos += fSmallLineHeight + 2 * PenWidth + fLineOffset; // set fCurrentYPos to new value

            QList<PColumnModel> lstForeignKeys;
            m_model->columns().getColumnsForConstraintType(Constraint::CT_ForeignKey, lstForeignKeys);
            foreach (const PColumnModel c, lstForeignKeys)
            {
                PConstraint cn = c->constraints().constraint(Constraint::CT_ForeignKey);
                if (cn->type() != Constraint::CT_Unknown)
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
        if (constraintTypeCounters[Constraint::CT_PrimaryKey] > 0)
        {
            // draw backward gradient
            fCurrentYPos += fLineOffset;
            painter->setBrush(brushBackward);
            painter->setPen(penGray);
            QRect columnTitleRect(innerBoundingRect.left(), fCurrentYPos, innerBoundingRect.width(), fSmallLineHeight + 2 * PenWidth);
            painter->drawRect(columnTitleRect);
            // printing "PK"
            painter->setFont(smallFont);
            painter->drawText(columnTitleRect, Qt::AlignVCenter | Qt::TextSingleLine | Qt::TextDontClip ,"  " + tr("PK"));
            fCurrentYPos += fSmallLineHeight + 2 * PenWidth + fLineOffset; // set fCurrentYPos to new value

            QList<PColumnModel> lstPrimaryKeys;
            m_model->columns().getColumnsForConstraintType(Constraint::CT_PrimaryKey, lstPrimaryKeys);

            QString sConstraintUMLString;
            foreach (const PColumnModel c, lstPrimaryKeys)
            {
                PConstraint cn = c->constraints().constraint(Constraint::CT_PrimaryKey);
                if (!cn->name().isEmpty())
                {
                    sConstraintUMLString = cn->getUMLConstraintString();
                }
            }

            painter->setFont(fontNormal);
            painter->setPen(penBlack);
            painter->drawText(QRect(innerBoundingRect.left() + PenWidth, fCurrentYPos, fColumnPrefixWidth, fLineHeight), Qt::AlignVCenter, "+");
            painter->drawText(QRect(innerBoundingRect.left() + fColumnPrefixWidth, fCurrentYPos, innerBoundingRect.width() - fColumnPrefixWidth, fLineHeight), Qt::AlignVCenter | Qt::TextSingleLine | Qt::TextDontClip, sConstraintUMLString);
            fCurrentYPos += fLineHeight; // set fCurrentYPos to new value
        }
        // Unique section
        if (constraintTypeCounters[Constraint::CT_Unique] > 0)
        {
            // draw backward gradient
            fCurrentYPos += fLineOffset;
            painter->setBrush(brushBackward);
            painter->setPen(penGray);
            QRect columnTitleRect(innerBoundingRect.left(), fCurrentYPos, innerBoundingRect.width(), fSmallLineHeight + 2 * PenWidth);
            painter->drawRect(columnTitleRect);
            // printing "unique"
            painter->setFont(smallFont);
            painter->drawText(columnTitleRect, Qt::AlignVCenter | Qt::TextSingleLine | Qt::TextDontClip ,"  " + tr("unique"));
            fCurrentYPos += fSmallLineHeight + 2 * PenWidth + fLineOffset; // set fCurrentYPos to new value

            QList<PColumnModel> lstUnique;
            m_model->columns().getColumnsForConstraintType(Constraint::CT_Unique, lstUnique);
            foreach (const PColumnModel c, lstUnique)
            {
                PConstraint cn = c->constraints().constraint(Constraint::CT_Unique);
                if (cn->type() != Constraint::CT_Unknown)
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

    int constraintTypeCounters[Constraint::CT_Last];
    qFill(&constraintTypeCounters[0], &constraintTypeCounters[Constraint::CT_Last], 0 );

    if (nColumnAmount > 0)
    {
        constraintTypeCounters[Constraint::CT_PrimaryKey] = m_model->columns().getAmountForType(Constraint::CT_PrimaryKey);
        constraintTypeCounters[Constraint::CT_Unique] = m_model->columns().getAmountForType(Constraint::CT_Unique);
        constraintTypeCounters[Constraint::CT_ForeignKey] = m_model->columns().getAmountForType(Constraint::CT_ForeignKey);
        constraintTypeCounters[Constraint::CT_Default] = m_model->columns().getAmountForType(Constraint::CT_Default);
        constraintTypeCounters[Constraint::CT_NotNull] = m_model->columns().getAmountForType(Constraint::CT_NotNull);
        constraintTypeCounters[Constraint::CT_Check] = m_model->columns().getAmountForType(Constraint::CT_Check);

        bool bHasConstraints = false;
        for (int i = 0; i < Constraint::CT_Last; i++)
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
        if (constraintTypeCounters[Constraint::CT_ForeignKey] > 0)
        {
            m_minHeight += fSmallLineHeight + 2 * PenWidth + fLineOffset;
            m_minHeight += (qreal)constraintTypeCounters[Constraint::CT_ForeignKey] * fLineHeight;
        }
        if (constraintTypeCounters[Constraint::CT_PrimaryKey] > 0)
        {
            m_minHeight += fSmallLineHeight + 2 * PenWidth + fLineOffset;
            m_minHeight += /* (qreal)constraintTypeCounters[Constraint::CT_PrimaryKey] * */ fLineHeight; // because PK is always ONE (might be not simple, but one)
        }
        if (constraintTypeCounters[Constraint::CT_Unique] > 0)
        {
            m_minHeight += fSmallLineHeight + 2 * PenWidth + fLineOffset;
            m_minHeight += (qreal)constraintTypeCounters[Constraint::CT_Unique] * fLineHeight;
        }

        // calculate width
        m_minWidth = qMax(longestStringWidth(metrics) + 2 * fColumnPrefixWidth, m_minWidth);
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

void TableWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete)
    {
        // delete widget
        emit deleteWidget(m_name);
    }
}

void TableWidget::setModel(PTableModel model)
{
    m_model = model;
    recalcMinimumSize(); // TODO: optimisation remove this line from here or from boundingRect() function
    update(boundingRect());
}

qreal TableWidget::longestStringWidth(const QFontMetrics& metrics) const
{
    qreal maxWidth = 0;
    foreach (const SharedColumnModel& column, m_model->columns())
    {
        maxWidth = qMax(maxWidth, (qreal)metrics.boundingRect(column->getUMLColumnDescription()).width());
        foreach (const SharedConstraint& cn, column->constraints())
        {
            if ((cn->type() != Constraint::CT_Unknown) && (!cn->name().isEmpty()))
            {
                maxWidth = qMax(maxWidth, (qreal)metrics.boundingRect(cn->getUMLConstraintString()).width());
            }
        }
    }

    return maxWidth;
}
