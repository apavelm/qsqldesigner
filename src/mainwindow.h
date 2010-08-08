#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QMimeData>
#include <QtGui/QClipboard>
#include <QtGui/QCloseEvent>
#include <QtGui/QGraphicsItemGroup>
#include <QtGui/QGraphicsScene>
#include <QtGui/QMainWindow>
#include <QtGui/QPrinter>

#include "widgets/mainview.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);

public slots:
    void setDirty(bool on = true);
    void clear();

private:
    void createSceneAndView();
    void createActions();
    void createToolBars();
    void createStatusBar();
    void copyItems(const QList<QGraphicsItem*> &items);

    Ui::MainWindow *ui;
    MainView * m_mainView;
    QPrinter * m_printer;
    QGraphicsScene * m_scene;
    QGraphicsItemGroup * m_gridGroup;

private slots:
    void slotNewProject();
    void slotOpenProject();
    void slotCloseProject();

    void slotEditCopy();
    void slotEditCut();
    void slotEditPaste();

    void slotViewShowGrid(bool on);
};

#endif // MAINWINDOW_H
