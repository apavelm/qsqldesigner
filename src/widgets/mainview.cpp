#include "mainview.h"
#include "../settingsmanager.h"
#include <cmath>

MainView::MainView(QWidget * parent) : QGraphicsView(parent)
{
    setDragMode(QGraphicsView::RubberBandDrag);
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setCacheMode(QGraphicsView::CacheBackground);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    m_cornerButton = new CornerZoomWidget(this);
    m_cornerButton->setPixmap(QPixmap(":/zoom"));
    m_cornerButton->setPixmapSize(verticalScrollBar()->sizeHint().width());
    connect(m_cornerButton, SIGNAL(clicked()), this, SIGNAL(cornerWidgetClicked()));
    setCornerWidget(m_cornerButton);
}

MainView::~MainView()
{
    delete m_cornerButton;
}

void MainView::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->save();
    QLinearGradient backgroundGradient(rect.left(),rect.top(),rect.left(),rect.bottom());
    backgroundGradient.setColorAt(0, SM->paperColor());
    backgroundGradient.setColorAt(1, QColor(Qt::white));
    painter->setPen(QPen(Qt::NoPen));
    painter->setBrush(QBrush(backgroundGradient));
    painter->drawRect(rect);
    painter->restore();
}

void MainView::scaleBy(qreal scaleFactor)
{
    qreal curScaleFactor = transform().m11();
    if (((curScaleFactor == minScale) && (scaleFactor < 1.0)) ||
        ((curScaleFactor == maxScale) && (scaleFactor > 1.0))) return;

    qreal sc = scaleFactor;

    if ((curScaleFactor * sc < minScale)&&(sc < 1.0))
    {
        sc = minScale / curScaleFactor;
    }
    else
        if ((curScaleFactor * sc > maxScale)&&(sc > 1.0))
        {
        sc = maxScale / curScaleFactor;
    }
    scale(sc, sc);
}

void MainView::setZoom(int percentZoom)
{
    qreal targetScale = (qreal)percentZoom / 100.0;
    qreal scaleFactor = targetScale / transform().m11();
    scaleBy(scaleFactor);
}

void MainView::wheelEvent(QWheelEvent *event)
{
   if (event->modifiers() == Qt::ControlModifier)
    {
        scaleBy(std::pow(4.0 / 3.0, (-event->delta() / 240.0)));
    }
    else
    {
        QGraphicsView::wheelEvent(event);
    }
}

void MainView::zoomIn()
{
    scaleBy(1.1);
}

void MainView::zoomOut()
{
    scaleBy(1.0 / 1.1);
}
