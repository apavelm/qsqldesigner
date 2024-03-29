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

#include "pluginmanager.h"
#include "projectmanager.h"
#include "settingsmanager.h"

#include <QDir>
#include <QFileDialog>

#include "aboutdialog.h"
#include "tabledialog.h"
#include "newprojectdialog.h"

#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
            QMainWindow(parent),
            ui(new Ui::MainWindow),
            m_zoomSignalMapper(new QSignalMapper(this))
{
    ui->setupUi(this);
    m_objEditor = new ObjectEditor(this);
    ui->propertyDockWidget->setWidget(m_objEditor);

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
    connect(ui->actionClose_All, SIGNAL(triggered()), PROJECTMANAGER, SLOT(closeAllProjects()));

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
    connect(ui->actionZoom_10, SIGNAL(triggered()), m_zoomSignalMapper.data(), SLOT(map()));
    m_zoomSignalMapper->setMapping(ui->actionZoom_10, 10);
    connect(ui->actionZoom_20, SIGNAL(triggered()), m_zoomSignalMapper.data(), SLOT(map()));
    m_zoomSignalMapper->setMapping(ui->actionZoom_20, 20);
    connect(ui->actionZoom_33, SIGNAL(triggered()), m_zoomSignalMapper.data(), SLOT(map()));
    m_zoomSignalMapper->setMapping(ui->actionZoom_33, 33);
    connect(ui->actionZoom_50, SIGNAL(triggered()), m_zoomSignalMapper.data(), SLOT(map()));
    m_zoomSignalMapper->setMapping(ui->actionZoom_50, 50);
    connect(ui->actionZoom_75, SIGNAL(triggered()), m_zoomSignalMapper.data(), SLOT(map()));
    m_zoomSignalMapper->setMapping(ui->actionZoom_75, 75);
    connect(ui->actionZoom_100, SIGNAL(triggered()), m_zoomSignalMapper.data(), SLOT(map()));
    m_zoomSignalMapper->setMapping(ui->actionZoom_100, 100);
    connect(ui->actionZoom_200, SIGNAL(triggered()), m_zoomSignalMapper.data(), SLOT(map()));
    m_zoomSignalMapper->setMapping(ui->actionZoom_200, 200);
    connect(m_zoomSignalMapper.data(), SIGNAL(mapped(int)), m_mainView, SLOT(setZoom(int)));

    // Project
    connect(ui->actionAdd_Table, SIGNAL(triggered()), this, SLOT(slotProjectAddTable()));

    // About
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(slotAboutAbout()));
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    connect(m_mainView, SIGNAL(copyAvailable(bool)), ui->actionCut, SLOT(setEnabled(bool)));
    connect(m_mainView, SIGNAL(copyAvailable(bool)), ui->actionCopy, SLOT(setEnabled(bool)));
    connect(QApplication::clipboard(), SIGNAL(dataChanged()), SLOT(setPasteActionEnabled()));

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
    QFileDialog dlgOpen(this);
    dlgOpen.setDirectory(QDir::home());
    dlgOpen.setWindowTitle(tr("Open project..."));
    dlgOpen.setNameFilter(tr("SQL Designer projects (*.sdp)"));
    dlgOpen.setFileMode(QFileDialog::ExistingFile);
    dlgOpen.setOption(QFileDialog::ReadOnly);
    dlgOpen.setDefaultSuffix(tr("sdp"));
    if (dlgOpen.exec() == QDialog::Accepted)
    {
        QString fileName = dlgOpen.selectedFiles().at(0);
        PROJECTMANAGER->openProject(fileName);
    }
}

void MainWindow::slotSave()
{
    if (CURRENTPROJECT)
    {
        if (!CURRENTPROJECT->projectFileName().isEmpty())
        {
            CURRENTPROJECT->saveProject(CURRENTPROJECT->projectFileName());
        }
        else
        {
            slotSaveAs();
        }
    }
}

void MainWindow::slotSaveAs()
{
    QFileDialog dlgSave(this);
    dlgSave.setDirectory(QDir::home());
    dlgSave.setWindowTitle(tr("Save project as..."));
    dlgSave.setNameFilter(tr("SQL Designer projects (*.sdp)"));
    dlgSave.setFileMode(QFileDialog::AnyFile);
    dlgSave.setDefaultSuffix(tr("sdp"));
    if (dlgSave.exec() == QDialog::Accepted)
    {
        QString fileName = dlgSave.selectedFiles().at(0);
        if (CURRENTPROJECT)
        {
            CURRENTPROJECT->saveProject(fileName);
        }
    }
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
    QList<QGraphicsItem*> items = CURRENTPROJECT->scene()->selectedItems();
    if (!items.isEmpty())
    {
        copyItems(items);
    }
}


void MainWindow::copyItems(const QList<QGraphicsItem*> &items)
{
    QByteArray copiedItems;
    QDataStream out(&copiedItems, QIODevice::WriteOnly);
    foreach(QGraphicsItem* item, items)
    {
        if (item->type() == TableWidget::Type)
        {
            PTableWidget pTableWidget = qgraphicsitem_cast<PTableWidget>(item);
            copiedItems += XmlHelper::serializeTableWidget(pTableWidget);
        }
    }
    QMimeData * mimeData = new QMimeData();
    mimeData->setData("application/x-qt-windows-mime;value=qsqldesigner-table", copiedItems);
    QClipboard * clipboard = QApplication::clipboard();
    clipboard->setMimeData(mimeData);
}


void MainWindow::slotEditCut()
{
    QList<QGraphicsItem*> items = CURRENTPROJECT->scene()->selectedItems();
    if (!items.isEmpty())
    {
        copyItems(items);
    }
    // TODO: remove items
    /*QListIterator<QGraphicsItem*> i(items);
    while (i.hasNext())
    {
        QScopedPointer<QGraphicsItem> item(i.next());
        scene->removeItem(item.data());
    }*/
}

void MainWindow::setPasteActionEnabled()
{
    QClipboard * clipboard = qApp->clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    if (CURRENTPROJECT)
    {
        if (mimeData->hasFormat("application/x-qt-windows-mime;value=qsqldesigner-table"))
        {
            ui->actionPaste->setEnabled(true);
            return;
        }
    }

    ui->actionPaste->setEnabled(false);
}

void MainWindow::slotEditPaste()
{
    QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    if (!mimeData)
        return;

    if (mimeData->hasFormat("application/x-qt-windows-mime;value=qsqldesigner-table"))
    {
        QByteArray copiedItems = mimeData->data("application/x-qt-windows-mime;value=qsqldesigner-table");
        QList<QPair<QString, QPointF> > coords;
        PTableModel table = XmlHelper::deserealizeTableWidget(copiedItems, CURRENTPROJECT->modelManager(), CURRENTPROJECT->dbmsType(), coords);
        CURRENTPROJECT->modelManager()->addTable(table);
    }
    /*else if (mimeData->hasHtml() || mimeData->hasText()) {
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

bool MainWindow::openTableDialog(PTableModel table)
{
    if(table)
    {
        TableDialog dlg(table, this);
        return dlg.exec() == QDialog::Accepted;
    }
    return false;
}

void MainWindow::slotProjectAddTable()
{
    if (CURRENTPROJECT)
    {
        PTableModel table = new TableModel(CURRENTPROJECT->modelManager());
        if (openTableDialog(table))
        {
            CURRENTPROJECT->modelManager()->addTable(table);
        }
        else
        {
            delete table;
        }
    }
}

void MainWindow::slotProjectEditTable(const QString& tableName)
{
    // TODO: this variant save table even user CANCEL operation.
    //PTableModel table = CURRENTPROJECT->modelManager()->getTableByName(tableName);
    //openTableDialog(table);
}

void MainWindow::slotAboutAbout()
{
    AboutDialog dlg(this);
    dlg.exec();
}

void MainWindow::slotCurrentProjectChange(const QString& projectName)
{
    if (!projectName.isEmpty())
    {
        if (CURRENTPROJECT)
        {
            setWindowTitle(QString("%1 - %2").arg(tr("SQL Designer")).arg(projectName));
            m_mainView->setScene(CURRENTPROJECT->scene());
            m_objEditor->setProject(CURRENTPROJECT);
            connect(CURRENTPROJECT, SIGNAL(modelChanged()), m_objEditor, SLOT(updateModel()));
            connect(CURRENTPROJECT, SIGNAL(editTable(QString)), this, SLOT(slotProjectEditTable(QString)));
            setPasteActionEnabled();
        }
    }
    else
    {
        setWindowTitle(tr("SQL Designer"));
        m_mainView->setScene(0);
        m_objEditor->setProject(0);
    }
}


#include <QMessageBox>
void MainWindow::on_actionGenerate_DDL_triggered()
{
    if (CURRENTPROJECT)
    {
        PSqlDesignerProject proj = CURRENTPROJECT;
        QString ddlScript = PLUGINMANAGER->pluginForDatabase(proj->dbmsType())->generateDDL(proj->modelManager());
        QMessageBox::information(this, "DDL Script", ddlScript, QMessageBox::Ok, QMessageBox::Ok);
    }
}
