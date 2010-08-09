#ifndef COMMANDS_H
#define COMMANDS_H

#include <QtGui/QGraphicsItem>
#include <QtGui/QGraphicsScene>
#include <QtGui/QUndoCommand>

class MoveCommand : public QUndoCommand
{
public:
    enum { Id = 1234 };

    MoveCommand(QGraphicsItem *diagramItem, const QPointF &oldPos,
                QUndoCommand *parent = 0);

    void undo();
    void redo();
    bool mergeWith(const QUndoCommand *command);
    int id() const { return Id; }

private:
    QGraphicsItem *myDiagramItem;
    QPointF myOldPos;
    QPointF newPos;
};

class DeleteCommand : public QUndoCommand
{
public:
    DeleteCommand(QGraphicsScene *graphicsScene, QUndoCommand *parent = 0);

    void undo();
    void redo();

private:
    QGraphicsItem *myDiagramItem;
    QGraphicsScene *myGraphicsScene;
};

class AddCommand : public QUndoCommand
{
public:
    AddCommand(int addType, QGraphicsScene *graphicsScene,
               QUndoCommand *parent = 0);

    void undo();
    void redo();

private:
    QGraphicsItem *myDiagramItem;
    QGraphicsScene *myGraphicsScene;
    QPointF initialPosition;
};

QString createCommandString(QGraphicsItem *item, const QPointF &point);

#endif //COMMANDS_H
