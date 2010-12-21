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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QMimeData>
#include <QtCore/QScopedPointer>
#include <QtCore/QSignalMapper>
#include <QtGui/QClipboard>
#include <QtGui/QCloseEvent>
#include <QtGui/QMainWindow>

#include "magnifydialog.h"
#include "widgets/mainview.h"
#include "models/table.h"
#include "widgets/objecteditor.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

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
    QScopedPointer<QSignalMapper> m_zoomSignalMapper;
    bool m_showGrid;
    ObjectEditor * m_objEditor;

    bool openTableDialog(PTableModel table);
private slots:
    void on_actionGenerate_DDL_triggered();
    void slotNewProject();
    void slotOpenProject();
    void slotSave();
    void slotSaveAs();
    void slotPrint();

    void slotEditUndo();
    void slotEditRedo();
    void slotEditCut();
    void slotEditCopy();
    void setPasteActionEnabled();
    void slotEditPaste();
    void slotEditSelectAll();

    void slotViewCustomZoom();

    void slotProjectAddTable();
    void slotProjectEditTable(const QString& tableName);

    void slotAboutAbout();

    void slotCurrentProjectChange(const QString& projectName);
};

#endif // MAINWINDOW_H
