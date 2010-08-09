#include "mainwindow.h"
#include "tabledialog.h"
#include "widgets/tablewidget.h"

#include "ui_mainwindow.h"

#include <cmath>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), m_gridGroup(0)
{
    ui->setupUi(this);
    m_printer = new QPrinter(QPrinter::HighResolution);
    m_undoStack = new QUndoStack();

    createSceneAndView();
    createMenus();
    createActions();
    createToolBars();
    createStatusBar();

    clear();
}

void MainWindow::createSceneAndView()
{
    m_mainView = new MainView;
    m_scene = new QGraphicsScene(this);
    QSize pageSize = m_printer->paperSize(QPrinter::Point).toSize();
    m_scene->setSceneRect(0, 0, pageSize.width(), pageSize.height());
    m_mainView->setScene(m_scene);
    setCentralWidget(m_mainView);
}

void MainWindow::createMenus()
{

}

MainWindow::~MainWindow()
{
    delete m_undoStack;
    delete m_printer;
    delete m_gridGroup;
    delete m_mainView;
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::setDirty(bool on)
{
    setWindowModified(on);
    //updateUi();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void MainWindow::createActions()
{
    // File
    ui->actionNew_Project->setShortcuts(QKeySequence::New);
    connect (ui->actionNew_Project, SIGNAL(triggered()), this, SLOT(slotNewProject()));
    ui->actionOpen_Project->setShortcuts(QKeySequence::Open);
    connect(ui->actionOpen_Project, SIGNAL(triggered()), this, SLOT(slotOpenProject()));

   /*saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));
*/

    // Edit
    ui->actionUndo->setShortcuts(QKeySequence::Undo);
    connect(ui->actionUndo, SIGNAL(triggered()), this, SLOT(slotEditUndo()));
    ui->actionRedo->setShortcuts(QKeySequence::Redo);
    connect(ui->actionRedo, SIGNAL(triggered()), this, SLOT(slotEditRedo()));
    ui->actionCut->setShortcuts(QKeySequence::Cut);
    connect(ui->actionCut, SIGNAL(triggered()), this, SLOT(slotEditCut()));
    ui->actionCopy->setShortcuts(QKeySequence::Copy);
    connect(ui->actionCopy, SIGNAL(triggered()), this, SLOT(slotEditCopy()));
    ui->actionPaste->setShortcuts(QKeySequence::Paste);
    connect(ui->actionPaste, SIGNAL(triggered()), this, SLOT(slotEditPaste()));

    // View
    connect(ui->actionShow_Grid, SIGNAL(toggled(bool)), this, SLOT(slotViewShowGrid(bool)));
    ui->actionZoom_In->setShortcuts(QKeySequence::ZoomIn);
    connect(ui->actionZoom_In, SIGNAL(triggered()), m_mainView, SLOT(zoomIn()));
    ui->actionZoom_Out->setShortcuts(QKeySequence::ZoomOut);
    connect(ui->actionZoom_Out, SIGNAL(triggered()), m_mainView, SLOT(zoomOut()));
    connect(ui->action_Custom_Zoom, SIGNAL(triggered()), this, SLOT(slotViewCustomZoom()));
    connect(m_mainView, SIGNAL(cornerWidgetClicked()), this, SLOT(slotViewCustomZoom()));

    // Project
    connect(ui->actionAdd_Table, SIGNAL(triggered()), this, SLOT(slotProjectAddTable()));

    // About
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(slotAboutAbout()));
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    /*cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    connect(textEdit, SIGNAL(copyAvailable(bool)),
            cutAct, SLOT(setEnabled(bool)));
    connect(textEdit, SIGNAL(copyAvailable(bool)),
            copyAct, SLOT(setEnabled(bool)));*/
}

void MainWindow::createToolBars()
{
    ui->mainToolBar->addAction(ui->actionAdd_Table);
}

void MainWindow::createStatusBar()
{
    ui->statusbar->showMessage(tr("Ready"));
}

void MainWindow::slotNewProject()
{
    clear();
    setWindowFilePath(tr("Untitled"));
    setDirty(false);
}

void MainWindow::slotOpenProject()
{

}

void MainWindow::slotCloseProject()
{

}

void MainWindow::slotEditUndo()
{
    /*QUndoCommand *deleteCommand = new DeleteCommand(diagramScene);
         undoStack->push(deleteCommand);

    QUndoCommand *addCommand = new AddCommand(DiagramItem::Box, diagramScene);
         undoStack->push(addCommand);

     undoStack->push(new MoveCommand(movedItem, oldPosition));*/
}

void MainWindow::slotEditRedo()
{

}

void MainWindow::slotEditCopy()
{
 /*   QList<QGraphicsItem*> items = scene->selectedItems();
    if (items.isEmpty())
        return;
    pasteOffset = OffsetIncrement;
    copyItems(items);
    updateUi();*/
}


void MainWindow::copyItems(const QList<QGraphicsItem*> &items)
{
    /*QByteArray copiedItems;
    QDataStream out(&copiedItems, QIODevice::WriteOnly);
    writeItems(out, items);
    QMimeData * mimeData = new QMimeData;
    mimeData->setData(MimeType, copiedItems);
    QClipboard * clipboard = QApplication::clipboard();
    clipboard->setMimeData(mimeData);*/
}


void MainWindow::slotEditCut()
{
  /*  QList<QGraphicsItem*> items = scene->selectedItems();
    if (items.isEmpty())
        return;
    copyItems(items);
    QListIterator<QGraphicsItem*> i(items);
    while (i.hasNext())
    {
        QScopedPointer<QGraphicsItem> item(i.next());
        scene->removeItem(item.data());
    }*/
    setDirty(true);
}


void MainWindow::slotEditPaste()
{
    QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    if (!mimeData)
        return;

    /*if (mimeData->hasFormat(MimeType)) {
        QByteArray copiedItems = mimeData->data(MimeType);
        QDataStream in(&copiedItems, QIODevice::ReadOnly);
        readItems(in, pasteOffset, true);
        pasteOffset += OffsetIncrement;
    }
    else if (mimeData->hasHtml() || mimeData->hasText()) {
        TextItem *textItem = new TextItem(position(), scene);
        connectItem(textItem);
        if (mimeData->hasHtml())
            textItem->setHtml(mimeData->html());
        else
            textItem->setPlainText(mimeData->text());
    }
    else
        return;
        */
    setDirty(true);
}

void MainWindow::slotViewCustomZoom()
{
    MagnifyDialog dlg;
    qreal fScale = 1.0; // REWRTIE
    dlg.setValue( fScale * 100 );
    if (dlg.exec() == QDialog::Accepted)
    {
        qreal zoomValue = (qreal)dlg.value() / 100.0;
    }
}

void MainWindow::slotViewShowGrid(bool on)
{
    if (!m_gridGroup)
    {
        const int GridSize = 40;
        QPen pen(QColor(175, 175, 175, 127));
        m_gridGroup = new QGraphicsItemGroup;
        const int MaxX = static_cast<int>(std::ceil(m_scene->width())  / GridSize) * GridSize;
        const int MaxY = static_cast<int>(std::ceil(m_scene->height()) / GridSize) * GridSize;
        for (int x = 0; x <= MaxX; x += GridSize)
        {
            QGraphicsLineItem *item = new QGraphicsLineItem(x, 0, x, MaxY);
            item->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
            item->setPen(pen);
            item->setZValue(-1000);
            m_gridGroup->addToGroup(item);
        }
        for (int y = 0; y <= MaxY; y += GridSize)
        {
            QGraphicsLineItem *item = new QGraphicsLineItem(0, y, MaxX, y);
            item->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
            item->setPen(pen);
            item->setZValue(-1000);
            m_gridGroup->addToGroup(item);
        }
        m_gridGroup->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
        m_scene->addItem(m_gridGroup);
    }
    m_gridGroup->setVisible(on);
}

void MainWindow::slotProjectAddTable()
{
    TableDialog dlg;
    if (dlg.exec() == QDialog::Accepted)
    {
        // add Table
        TableWidget * w = new TableWidget(m_scene, 0, new TableModel("test_table"));
    }
}

bool MainWindow::sceneHasItems() const
{
    foreach (QGraphicsItem *item, m_scene->items())
        if (item != m_gridGroup && item->group() != m_gridGroup)
            return true;
    return false;
}

void MainWindow::clear()
{
    slotViewShowGrid(ui->actionShow_Grid->isChecked());
}

void MainWindow::slotAboutAbout()
{

}

