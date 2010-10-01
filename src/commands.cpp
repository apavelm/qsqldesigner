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

#include "commands.h"

MoveCommand::MoveCommand(QGraphicsItem *diagramItem, const QPointF &oldPos, QUndoCommand *parent) : QUndoCommand(parent)
{
    myDiagramItem = diagramItem;
    newPos = diagramItem->pos();
    myOldPos = oldPos;
}

bool MoveCommand::mergeWith(const QUndoCommand *command)
{
    const MoveCommand *moveCommand = static_cast<const MoveCommand *>(command);
    QGraphicsItem *item = moveCommand->myDiagramItem;

    if (myDiagramItem != item)
    return false;

    newPos = item->pos();
    setText(QObject::tr("Move %1")
        .arg(createCommandString(myDiagramItem, newPos)));

    return true;
}

void MoveCommand::undo()
{
    myDiagramItem->setPos(myOldPos);
    myDiagramItem->scene()->update();
    setText(QObject::tr("Move %1")
        .arg(createCommandString(myDiagramItem, newPos)));
}

void MoveCommand::redo()
{
    myDiagramItem->setPos(newPos);
    setText(QObject::tr("Move %1")
        .arg(createCommandString(myDiagramItem, newPos)));
}

DeleteCommand::DeleteCommand(QGraphicsScene *scene, QUndoCommand *parent) : QUndoCommand(parent)
{
    myGraphicsScene = scene;
    QList<QGraphicsItem *> list = myGraphicsScene->selectedItems();
    list.first()->setSelected(false);
    myDiagramItem = static_cast<QGraphicsItem *>(list.first());
    setText(QObject::tr("Delete %1")
        .arg(createCommandString(myDiagramItem, myDiagramItem->pos())));
}

void DeleteCommand::undo()
{
    myGraphicsScene->addItem(myDiagramItem);
    myGraphicsScene->update();
}

void DeleteCommand::redo()
{
    myGraphicsScene->removeItem(myDiagramItem);
}

AddCommand::AddCommand(int addType, QGraphicsScene *scene, QUndoCommand *parent) : QUndoCommand(parent)
{
    static int itemCount = 0;

    myGraphicsScene = scene;
    //myDiagramItem = new QGraphicsItem(addType);
    initialPosition = QPointF((itemCount * 15) % int(scene->width()),
                              (itemCount * 15) % int(scene->height()));
    scene->update();
    ++itemCount;
    setText(QObject::tr("Add %1")
        .arg(createCommandString(myDiagramItem, initialPosition)));
}

void AddCommand::undo()
{
    myGraphicsScene->removeItem(myDiagramItem);
    myGraphicsScene->update();
}

void AddCommand::redo()
{
    myGraphicsScene->addItem(myDiagramItem);
    myDiagramItem->setPos(initialPosition);
    myGraphicsScene->clearSelection();
    myGraphicsScene->update();
}

QString createCommandString(QGraphicsItem *item, const QPointF &pos)
{
    /*return QObject::tr("%1 at (%2, %3)")
        .arg(item->diagramType() == DiagramItem::Box ? "Box" : "Triangle")
        .arg(pos.x()).arg(pos.y());*/
    return QString();
}
