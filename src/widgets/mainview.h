#ifndef MAINVIEW_H
#define MAINVIEW_H

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
public slots:
    void zoomIn();
    void zoomOut();
    void setZoom(int);

protected:
    void wheelEvent(QWheelEvent * event);

private:
    CornerZoomWidget * m_cornerButton;
    void scaleBy(qreal scaleFactor);

signals:
    void cornerWidgetClicked();
};

#endif // MAINVIEW_H
