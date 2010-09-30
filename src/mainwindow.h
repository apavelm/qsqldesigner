#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QMimeData>
#include <QtCore/QSignalMapper>
#include <QtGui/QUndoStack>
#include <QtGui/QUndoView>
#include <QtGui/QClipboard>
#include <QtGui/QCloseEvent>
#include <QtGui/QMainWindow>
#include <QtGui/QPrinter>
#include <QtGui/QUndoCommand>

#include "magnifydialog.h"
#include "widgets/mainview.h"
#include "models/table.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);

private:
    void createSceneAndView();
    void createMenus();
    void createActions();
    void createToolBars();
    void createStatusBar();
    void copyItems(const QList<QGraphicsItem*> &items);

    Ui::MainWindow * ui;
    MainView * m_mainView;
    QPrinter * m_printer;
    QUndoStack * m_undoStack;
    QSignalMapper * m_zoomSignalMapper;
    bool m_showGrid;

private slots:
    void slotNewProject();
    void slotOpenProject();
    void slotSave();
    void slotSaveAs();
    void slotPrint();

    void slotEditUndo();
    void slotEditRedo();
    void slotEditCut();
    void slotEditCopy();
    void slotEditPaste();
    void slotEditSelectAll();

    void slotViewCustomZoom();

    void slotProjectAddTable();

    void slotAboutAbout();

    void slotCurrentProjectChange(const QString& projectName);
};

#endif // MAINWINDOW_H
