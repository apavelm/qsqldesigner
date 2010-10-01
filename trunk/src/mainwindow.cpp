/***************************************************************************
 *   Copyright (C) 2010 by Pavel Andreev                                   *
 *   Mail: apavelm on gmail point com (apavelm@gmail.com)                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, you can read it                      *
 *   here <http://www.gnu.org/licenses/>, or write to the                  *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "mainwindow.h"

#include "settingsmanager.h"
#include "projectmanager.h"
#include "tabledialog.h"
#include "newprojectdialog.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_printer = new QPrinter(QPrinter::HighResolution);
    m_undoStack = new QUndoStack();
    m_zoomSignalMapper = new QSignalMapper(this);

    createSceneAndView();
    createMenus();
    createActions();
    createToolBars();
    createStatusBar();
}

void MainWindow::createSceneAndView()
{
    m_mainView = new MainView;
    setCentralWidget(m_mainView);
}

void MainWindow::createMenus()
{

}

MainWindow::~MainWindow()
{
    delete m_undoStack;
    delete m_printer;
    delete m_mainView;
    delete m_zoomSignalMapper;
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
    connect(ui->actionClose_Project, SIGNAL(triggered()), PROJECTMANAGER, SLOT(closeProject()));

    ui->actionSave->setShortcuts(QKeySequence::Save);
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(slotSave()));
    ui->actionSave_As->setShortcuts(QKeySequence::SaveAs);
    connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(slotSaveAs()));
    ui->actionPrint->setShortcuts(QKeySequence::Print);
    connect(ui->actionPrint, SIGNAL(triggered()), this, SLOT(slotPrint()));

    ui->actionExit->setShortcut(QKeySequence::Quit);

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
    ui->actionSelect_All->setShortcuts(QKeySequence::SelectAll);
    connect(ui->actionSelect_All, SIGNAL(triggered()), this, SLOT(slotEditSelectAll()));

    // View
    connect(ui->actionShow_Grid, SIGNAL(toggled(bool)), SM, SLOT(setShowGrid(bool)));
    ui->actionZoom_In->setShortcuts(QKeySequence::ZoomIn);
    connect(ui->actionZoom_In, SIGNAL(triggered()), m_mainView, SLOT(zoomIn()));
    ui->actionZoom_Out->setShortcuts(QKeySequence::ZoomOut);
    connect(ui->actionZoom_Out, SIGNAL(triggered()), m_mainView, SLOT(zoomOut()));
    connect(ui->action_Custom_Zoom, SIGNAL(triggered()), this, SLOT(slotViewCustomZoom()));
    connect(m_mainView, SIGNAL(cornerWidgetClicked()), this, SLOT(slotViewCustomZoom()));
    connect(ui->actionZoom_10, SIGNAL(triggered()), m_zoomSignalMapper, SLOT(map()));
    m_zoomSignalMapper->setMapping(ui->actionZoom_10, 10);
    connect(ui->actionZoom_20, SIGNAL(triggered()), m_zoomSignalMapper, SLOT(map()));
    m_zoomSignalMapper->setMapping(ui->actionZoom_20, 20);
    connect(ui->actionZoom_33, SIGNAL(triggered()), m_zoomSignalMapper, SLOT(map()));
    m_zoomSignalMapper->setMapping(ui->actionZoom_33, 33);
    connect(ui->actionZoom_50, SIGNAL(triggered()), m_zoomSignalMapper, SLOT(map()));
    m_zoomSignalMapper->setMapping(ui->actionZoom_50, 50);
    connect(ui->actionZoom_75, SIGNAL(triggered()), m_zoomSignalMapper, SLOT(map()));
    m_zoomSignalMapper->setMapping(ui->actionZoom_75, 75);
    connect(ui->actionZoom_100, SIGNAL(triggered()), m_zoomSignalMapper, SLOT(map()));
    m_zoomSignalMapper->setMapping(ui->actionZoom_100, 100);
    connect(ui->actionZoom_200, SIGNAL(triggered()), m_zoomSignalMapper, SLOT(map()));
    m_zoomSignalMapper->setMapping(ui->actionZoom_200, 200);
    connect(m_zoomSignalMapper, SIGNAL(mapped(int)), m_mainView, SLOT(setZoom(int)));

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

    connect(PROJECTMANAGER, SIGNAL(currentProjectChanged(QString)), this, SLOT(slotCurrentProjectChange(QString)));

    connect(SM, SIGNAL(dirty()), m_mainView, SLOT(update()));
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
    NewProjectDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
        PROJECTMANAGER->newProject(dlg.projectName(), dlg.typeDBMS());
    }
}

void MainWindow::slotOpenProject()
{

}

void MainWindow::slotSave()
{

}

void MainWindow::slotSaveAs()
{

}

void MainWindow::slotPrint()
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
}

void MainWindow::slotEditSelectAll()
{

}

void MainWindow::slotViewCustomZoom()
{
    MagnifyDialog dlg;
    dlg.setValue( m_mainView->curScale() * 100 );
    if (dlg.exec() == QDialog::Accepted)
    {
        m_mainView->setZoom(dlg.value());
    }
}

void MainWindow::slotProjectAddTable()
{
    if (CURRENTPROJECT)
    {
        TableDialog dlg(this, CURRENTPROJECT);
        dlg.exec();
    }
}

void MainWindow::slotAboutAbout()
{

}

void MainWindow::slotCurrentProjectChange(const QString& projectName)
{
    setWindowTitle(QString("%1 - %2").arg(tr("SQL Designer")).arg(projectName));
    m_mainView->setScene(CURRENTPROJECT->scene());
}

