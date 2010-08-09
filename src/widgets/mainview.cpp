#include "mainview.h"

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

void MainView::scaleBy(double factor)
{
    scale(factor, factor);
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
