#ifndef CORNERZOOMWIDGET_H
#define CORNERZOOMWIDGET_H

#include <QtGui/QStylePainter>
#include <QtGui/QWidget>

class CornerZoomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CornerZoomWidget(QWidget * parent = 0);
    void setPixmap(const QPixmap& pixmap);
    void setPixmapSize(int size);
protected:
   void paintEvent(QPaintEvent *);
   void mouseMoveEvent(QMouseEvent *);
   void mousePressEvent(QMouseEvent *);
private:
    QPixmap m_pixmap;
    int m_size;
signals:
    void clicked();
};

#endif // CORNERZOOMWIDGET_H
