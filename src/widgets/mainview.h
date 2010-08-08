#ifndef MAINVIEW_H
#define MAINVIEW_H


#include <QtGui/QGraphicsView>
#include <QtGui/QWheelEvent>

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
    void scaleBy(double factor);
};

#endif // MAINVIEW_H
