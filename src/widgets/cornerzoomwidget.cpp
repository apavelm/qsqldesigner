#include "cornerzoomwidget.h"
#include "mainview.h"

CornerZoomWidget::CornerZoomWidget()
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
    QStyleOptionButton option;
    option.initFrom(this);
    option.state = QStyle::State_Raised;
    option.icon = QIcon(m_pixmap);
    option.iconSize = QSize(m_size - 4, m_size - 4);
    option.state |= QStyle::State_Enabled;

    QPainter painter(this);
    style()->drawControl(QStyle::CE_PushButton, &option, &painter, this);
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
