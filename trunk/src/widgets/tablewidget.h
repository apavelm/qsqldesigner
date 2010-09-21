#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QtCore/QtGlobal>
#include <QtCore/QSharedPointer>
#include <QtGui/QFontMetrics>
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

    explicit TableWidget(QGraphicsScene *scene, QGraphicsItem  * parent = 0, PTableModel model = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

    inline const QString& name() const {return m_name;}
    QSizeF recalcMinimumSize() const;

    void setModel(PTableModel model);
public slots:
    void setName(const QString& name);
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
private:
    Q_DISABLE_COPY ( TableWidget )
    PTableModel m_model;
    QString m_name;

    void paintSelectionOutline(QPainter *painter);
    qreal longestStringWidth(const QFontMetrics& metrics) const;
};

typedef TableWidget * PTableWidget;
typedef QSharedPointer<TableWidget> SharedTableWidget;

#endif // TABLEWIDGET_H
