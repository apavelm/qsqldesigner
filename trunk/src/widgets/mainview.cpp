#include "mainview.h"

#include <cmath>

MainView::MainView(QWidget * parent) : QGraphicsView(parent)
{
    setDragMode(QGraphicsView::RubberBandDrag);
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
}

MainView::~MainView()
{
}

void MainView::scaleBy(double factor)
{
    scale(factor, factor);
}

void MainView::wheelEvent(QWheelEvent *event)
{
    scaleBy(std::pow(4.0 / 3.0, (-event->delta() / 240.0)));
}

void MainView::zoomIn()
{
    scaleBy(1.1);
}

void MainView::zoomOut()
{
    scaleBy(1.0 / 1.1);
}
