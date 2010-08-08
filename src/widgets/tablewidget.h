#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtGui/QGraphicsObject>

#include "../models/table.h"

class TableWidget : public QGraphicsObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName)
public:
    explicit TableWidget(QGraphicsItem  * parent = 0, TableModel * model = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

    inline const QString& name() const {return m_model->name();}
public slots:
    void setName(const QString& name);
signals:
    void dirty();

private:
    Q_DISABLE_COPY ( TableWidget )
    TableModel * m_model;

    void paintSelectionOutline(QPainter *painter);
};

#endif // TABLEWIDGET_H
