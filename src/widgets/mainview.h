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
    MainView(QWidget * parent = 0);
    ~MainView();

public slots:
    void zoomIn();
    void zoomOut();

protected:
    void wheelEvent(QWheelEvent * event);

private:
    CornerZoomWidget * m_cornerButton;
    void scaleBy(double factor);
signals:
    void cornerWidgetClicked();
};

#endif // MAINVIEW_H
