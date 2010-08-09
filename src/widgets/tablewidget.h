#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtGui/QGraphicsObject>
#include <QtGui/QGraphicsScene>

#include "../models/table.h"

class TableWidget : public QGraphicsObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName)
public:
    enum { Type = UserType + 1 };
    inline int type() const {return Type;}

    explicit TableWidget(QGraphicsScene *scene, QGraphicsItem  * parent = 0, TableModel * model = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

    inline const QString& name() const {return m_model->name();}
public slots:
    void setName(const QString& name);
signals:
    void dirty();
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
private:
    Q_DISABLE_COPY ( TableWidget )
    TableModel * m_model;

    void paintSelectionOutline(QPainter *painter);
};

QDataStream &operator<<(QDataStream &out, const TableWidget &item);
QDataStream &operator>>(QDataStream &in, TableWidget &item);

#endif // TABLEWIDGET_H
