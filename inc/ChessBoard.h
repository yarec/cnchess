#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "ChessMan.h"
#include <QtGui/QColor>
#include <QtGui/QGraphicsItem>
#include <QFile>

class ChessBoard : public ChessGraphic
{
public:
    ChessBoard(int x, int y);
    ~ChessBoard();

    void initScene(QGraphicsScene &scene);
    QRectF boundingRect() const;
    QPainterPath shape() const;

    void start();
    void newGame();
    bool getPcTurn(){ return bPcTurn;}

    void backward();
    void forward();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

private:
    int x, y;
    QColor color;
    QImage imgbg;
    QList<QPointF> stuff;

};

#endif
