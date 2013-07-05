
#ifndef CHIP_H
#define CHIP_H

#include <QtGui/QColor>
#include <QtGui/QGraphicsItem>
#include "chessgame.h"
#include <sstream>

class ChessGraphic : public QGraphicsItem
{
public:
    ChessGraphic();

    QPointF cmpos(int x, int y);
    QPointF cmpos(QPointF pos);
    QPointF cmpos(std::string pos);
    QPoint pos_int(QPointF pos);
    ChessGraphic* findCM(QPointF src);
    void reset();
    void moveCM(std::string iccs);
    void moveBackCm(QString moveii);
    bool tryMove(QPoint dstPos);
    void drawEngineThink(QPainter *painter);

    void setSelect(ChessGraphic *select);
    void setChessBoard(ChessGraphic *cb);
    void setInitPos(QPointF initPos);

    void asHover();
    void think();

    ChessGame               *cgame;
    ChessGraphic            *holdcm;
    ChessGraphic            *hover;
    ChessGraphic            *select;
    ChessGraphic            *lastMove;
    QList<ChessGraphic*>    *cms;
    QPoint                   srcPos;
    QPointF                  initPos;
    bool                     bPcTurn;
    bool                     bCheckedBy;
    bool                     bMate;
    bool                     bPcMate;
    int                      nCheckedBy;

    QList<QString>           backward_steps;
    QList<QString>           forward_steps;
protected:
    ChessGraphic* cb;
    qreal lw;
    qreal th;
    qreal cmw;
    qreal gw;
    qreal offset_l;
    qreal offset_t;

    bool ishover;

    QString           step_fen;
    int               cur_step_num;
};

class ChessMan : public ChessGraphic
{
public:
    ChessMan(const QString fname, int x, int y);
    void init(QString fname, int x, int y);


    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

private:
    int               x, y;
    QString           fname;
    QImage            image;
    QList<QPointF>    stuff;
};

#endif
