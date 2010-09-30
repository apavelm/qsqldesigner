#ifndef ARROWFOREIGNKEY_H
#define ARROWFOREIGNKEY_H

#include <QtCore/QList>
#include <QtCore/QSharedPointer>
#include <QtGui/QGraphicsObject>
#include <QtGui/QGraphicsPathItem>

#include "../models/constraint.h"
#include "../models/column.h"
#include "tablewidget.h"

class WidgetManager;
typedef WidgetManager * PWidgetManager;

class ArrowForeignKey : public QGraphicsPathItem
{
public:
    enum { Type = UserType + 2 };
    inline int type() const {return Type;}

    ArrowForeignKey(PWidgetManager manager, PConstraint constraint);
    ~ArrowForeignKey();

    QRectF boundingRect() const;
    QPainterPath shape() const;

    inline PTableWidget sourceTable() const { return m_sourceTable; }
    inline QList<QString> sourceColumns() const {return m_fk.sourceColumns();}
    inline PTableWidget refTable() const { return m_refTable; }
    inline QList<QString> refColumns() const {return m_fk.referenceColumns();}
    inline bool isValid() const {return m_initiated;}
public slots:
    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
    PConstraint  m_constraint;
    ConstraintForeignKey m_fk;
    PWidgetManager m_wm;
    bool         m_initiated;
    PTableWidget m_sourceTable;
    PTableWidget m_refTable;
    QPolygonF    m_arrowHead;
};

typedef ArrowForeignKey * PArrowForeignKey;
typedef QSharedPointer<ArrowForeignKey> SharedArrowForeignKey;
typedef QList<PArrowForeignKey> ListArrowForeignKey;

#endif // ARROWFOREIGNKEY_H
