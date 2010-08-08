#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cmath>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), m_gridGroup(0)
{
    ui->setupUi(this);
    m_printer = new QPrinter(QPrinter::HighResolution);

    createSceneAndView();
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

MainWindow::~MainWindow()
{
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
    // New Project
    ui->actionNew_Project->setShortcuts(QKeySequence::New);
    connect (ui->actionNew_Project, SIGNAL(triggered()), this, SLOT(slotNewProject()));

    // Open Project
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
    ui->actionCut->setShortcuts(QKeySequence::Cut);
    connect(ui->actionCut, SIGNAL(triggered()), this, SLOT(slotEditCut()));
    ui->actionCopy->setShortcuts(QKeySequence::Copy);
    connect(ui->actionCopy, SIGNAL(triggered()), this, SLOT(slotEditCopy()));
    ui->actionPaste->setShortcuts(QKeySequence::Paste);
    connect(ui->actionPaste, SIGNAL(triggered()), this, SLOT(slotEditPaste()));

    connect(ui->actionShow_Grid, SIGNAL(toggled(bool)), this, SLOT(slotViewShowGrid(bool)));
    //connect(ui->actionZoom_In, SIGNAL(triggered()), mainView, SLOT(zoomIn()));
    //connect(ui->actionZoom_Out, SIGNAL(triggered()), mainView, SLOT(zoomOut()));


    //connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
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
    /*fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);*/
}

void MainWindow::createStatusBar()
{
    ui->statusbar->showMessage(tr("Ready"));
}

void MainWindow::slotNewProject()
{

}

void MainWindow::slotOpenProject()
{

}

void MainWindow::slotCloseProject()
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
    while (i.hasNext()) {
#if QT_VERSION >= 0x040600
        QScopedPointer<QGraphicsItem> item(i.next());
        scene->removeItem(item.data());
#else
        QGraphicsItem *item = i.next();
        scene->removeItem(item);
        delete item;
#endif
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
            item->setPen(pen);
            item->setZValue(-1000);
            m_gridGroup->addToGroup(item);
        }
        for (int y = 0; y <= MaxY; y += GridSize)
        {
            QGraphicsLineItem *item = new QGraphicsLineItem(0, y, MaxX, y);
            item->setPen(pen);
            item->setZValue(-1000);
            m_gridGroup->addToGroup(item);
        }
        m_scene->addItem(m_gridGroup);
    }
    m_gridGroup->setVisible(on);
}

void MainWindow::clear()
{
    slotViewShowGrid(ui->actionShow_Grid->isChecked());
}
