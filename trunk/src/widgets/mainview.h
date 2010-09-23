#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QtCore/QScopedPointer>
#include <QtGui/QGraphicsView>
#include <QtGui/QScrollBar>
#include <QtGui/QToolButton>
#include <QtGui/QWheelEvent>

#include "cornerzoomwidget.h"

class MainView : public QGraphicsView
{
    Q_OBJECT
public:
    static const qreal minScale = 0.1;
    static const qreal maxScale = 5.0;

    MainView(QWidget * parent = 0);
    ~MainView();

    inline qreal curScale() const {return transform().m11();}
    void setRenderHintsAccordingSettings();
public slots:
    void zoomIn();
    void zoomOut();
    void setZoom(int);
    void update();

protected:
    void wheelEvent(QWheelEvent * event);

private:
    QScopedPointer<CornerZoomWidget> m_cornerButton;
    QVector<QPointF> m_gridPoints;

    void recalcGridPoints(const QRectF &rect);
    void scaleBy(qreal scaleFactor);
    void drawBackground(QPainter *painter, const QRectF &rect);
    void scrollContentsBy(int dx, int dy);

signals:
    void cornerWidgetClicked();
};

#endif // MAINVIEW_H
