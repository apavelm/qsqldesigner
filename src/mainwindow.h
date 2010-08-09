#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QMimeData>
#include <QtGui/QUndoStack>
#include <QtGui/QUndoView>
#include <QtGui/QClipboard>
#include <QtGui/QCloseEvent>
#include <QtGui/QGraphicsItemGroup>
#include <QtGui/QGraphicsScene>
#include <QtGui/QMainWindow>
#include <QtGui/QPrinter>
#include <QtGui/QUndoCommand>

#include "magnifydialog.h"
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
    void createMenus();
    void createActions();
    void createToolBars();
    void createStatusBar();
    void copyItems(const QList<QGraphicsItem*> &items);

    bool sceneHasItems() const;

    Ui::MainWindow * ui;
    MainView * m_mainView;
    QPrinter * m_printer;
    QGraphicsScene * m_scene;
    QGraphicsItemGroup * m_gridGroup;
    QUndoStack * m_undoStack;

private slots:
    void slotNewProject();
    void slotOpenProject();
    void slotCloseProject();

    void slotEditUndo();
    void slotEditRedo();
    void slotEditCut();
    void slotEditCopy();
    void slotEditPaste();

    void slotViewCustomZoom();
    void slotViewShowGrid(bool on);

    void slotProjectAddTable();

    void slotAboutAbout();
};

#endif // MAINWINDOW_H
