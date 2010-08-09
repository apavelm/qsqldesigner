#include "cornerzoomwidget.h"
#include "mainview.h"

CornerZoomWidget::CornerZoomWidget(QWidget *parent) : QWidget(parent)
{
    m_pixmap = QPixmap();
    m_size = 32;
    setStatusTip(tr("Click to change Zoom"));
    setMouseTracking(true);
}

void CornerZoomWidget::setPixmap(const QPixmap& pixmap)
{
    m_pixmap = pixmap;
}

void CornerZoomWidget::setPixmapSize(int size)
{
    m_size = size;
}

void CornerZoomWidget::paintEvent(QPaintEvent *)
{
    QStylePainter painter(this);
    painter.drawPixmap(QRect(0, 0, m_size, m_size), m_pixmap);
}

void CornerZoomWidget::mouseMoveEvent(QMouseEvent *)
{
    MainView * v = qobject_cast<MainView*>(parent());
    int zoomPercent = 100 * v->curScale();
    QString sZoom = tr("Current zoom: %1\%").arg(zoomPercent);
    setToolTip(sZoom);
}

void CornerZoomWidget::mousePressEvent(QMouseEvent *)
{
    emit clicked();
}
