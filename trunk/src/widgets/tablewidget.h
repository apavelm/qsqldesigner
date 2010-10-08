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

#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QtCore/QtGlobal>
#include <QtCore/QSharedPointer>
#include <QtGui/QKeyEvent>
#include <QtGui/QFontMetrics>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtGui/QGraphicsObject>
#include <QtGui/QGraphicsScene>

#include "../models/table.h"

class TableWidget : public QGraphicsObject
{
    Q_OBJECT
public:
    enum { Type = UserType + 1 };
    inline int type() const {return Type;}

    explicit TableWidget(QGraphicsScene *scene, QGraphicsItem  * parent = 0, PTableModel model = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

    inline const QString& name() const {return m_model->name();}
    QSizeF recalcMinimumSize() const;

    void setModel(PTableModel model);
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void keyPressEvent(QKeyEvent *event);
private:
    Q_DISABLE_COPY ( TableWidget )
    PTableModel m_model;

    void paintSelectionOutline(QPainter *painter);
    qreal longestStringWidth(const QFontMetrics& metrics) const;
signals:
    void deleteWidget(QString name);
    void editWidget(QString name);
    void updateWidget(QString name);
};

typedef TableWidget * PTableWidget;
typedef QSharedPointer<TableWidget> SharedTableWidget;

#endif // TABLEWIDGET_H
