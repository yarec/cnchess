#include "ChessMan.h"
#include "pocolog.h"
#include <QtGui>
#include <sstream>

ChessGraphic::ChessGraphic(){
    lw = 44.5;
    th = 45.5;
    cmw = 57;
    gw = 54;
    offset_l = lw-(cmw/2);
    offset_t = th-(cmw/2);
    ishover = false;
    cb = NULL;
    bCheckedBy = false;
    bMate= false;
    bPcMate= false;
}

QPointF ChessGraphic::cmpos(int x,int y)
{
    return QPointF( offset_l + x*gw, offset_t + y*gw);
}

QPointF ChessGraphic::cmpos(QPointF pos)
{
    int x = (pos.x()- offset_l) / gw;
    int y = (pos.y()- offset_t) / gw;
    return cmpos(x, y);
}

QPointF ChessGraphic::cmpos(std::string pos)
{
    int x = pos.c_str()[0]-'a';
    int y = '9'-pos.c_str()[1];
    return cmpos(x, y);
}

QPoint ChessGraphic::pos_int(QPointF pos)
{
    return QPoint((pos.x()- offset_l) / gw, (pos.y()- offset_t) / gw);
}

void ChessGraphic::asHover()
{
    ishover = true;
}

void ChessGraphic::think()
{
    cb->bPcTurn = true;
    cb->cgame->think(10);
    cb->nCheckedBy  = cb->cgame->CheckedBy();

    if(cb->cgame->is_nobestmove()){
        bPcMate = true;
        return;
    }
    moveCM( cb->cgame->get_bestmove() );


    cb->bPcTurn = false;
    cb->holdcm = NULL;

    if (cb->cgame->is_mate()) {
        cb->bMate = true;
        log("user is mate!");
    }
}

ChessGraphic* ChessGraphic::findCM(QPointF src)
{
    for (int i = 0; i < cb->cms->size(); ++i) {
        if(cb->cms->at(i)->pos() == src){
            return cb->cms->at(i);
        }
    }
    return NULL;
}

void ChessGraphic::reset()
{

    if(cgame != NULL) {
        delete cgame;
        cgame = NULL;
    }
    cgame = new ChessGame;

    if(!cms->isEmpty()){
        for (int i = 0; i < cms->size(); ++i) {
            ChessGraphic *cm = cms->at(i);
            cm->setPos(cm->initPos);
        }
    }

    bPcTurn = false;
    holdcm = NULL;
    nCheckedBy = 0;
    bCheckedBy = false;
    bPcMate= false;
    bMate= false;
    srcPos = QPoint(0,0);
}

void ChessGraphic::moveCM(std::string bestmove)
{
    const char* iccs = bestmove.c_str();
    QPointF src = cmpos( ((int)iccs[0]-97), (57-(int)iccs[1]));
    QPointF dst = cmpos( ((int)iccs[2]-97), (57-(int)iccs[3]));

    QString move = cb->cgame->get_bestmove().c_str();
    QString fen = cb->backward_steps.takeLast() +" "+ move;

    ChessGraphic* cm = findCM(dst);

    QString eated_cm = "";
    if( cm != NULL){
        eated_cm += QChar(cb->cms->indexOf(cm));
        cm->setPos(-70,0);
        cb->cgame->to_Fen();
    }

    cm = findCM(src);
    if(cm != NULL){
        fen += QChar(cb->cms->indexOf(cm));
        cb->lastMove = cm;
        cm->setPos(dst);
    }

    cb->backward_steps << fen + eated_cm;

}

void ChessGraphic::moveBackCm(QString moveii)
{
    // moveii is :
    // iccs            (4 byte)
    // idx of src cm   (1 byte)
    // idx of eated cm (1 byte)

    QPointF pos = cmpos( moveii.left(2).toStdString() );
    int idx = moveii.at(4).toAscii();
    cms->at(idx)->setPos(pos);

log(idx);
    if(moveii.length()==6){
        idx = moveii.at(5).toAscii();
        pos = cmpos( moveii.mid(2,2).toStdString() );
        cms->at(idx)->setPos(pos);
log(idx);
    }
}

bool ChessGraphic::tryMove(QPoint dstPos)
{
    cb->cgame->make_move(cb->srcPos.x(), cb->srcPos.y(), dstPos.x(), dstPos.y());
    if(cb->nCheckedBy!=0 && cb->cgame->CheckedBy()==cb->nCheckedBy) {
        cb->bCheckedBy = true;
        cb->cgame->Rollback();
        return false;
    }
    cb->holdcm->setPos(this->pos());
    cb->select->setPos(this->pos());
    return true;
}
void ChessGraphic::drawEngineThink(QPainter *painter)
{
    if(cb!=NULL && cb->cgame!=NULL){
        QFont font("Arial",20, QFont::Normal,true);
        painter->setFont(font);
        QStringList steps_split = QString(cb->cgame->get_depth()).split(" pv ");
        if(steps_split.size() > 1){
            QStringList step_split = steps_split[1].split(" ");
            int b = 0;
            int n = 0;
            int linewidth = 4;
            foreach(QString step , step_split){
                QPointF point_offset(gw/2, gw/2);
                QPointF src = cmpos(step.toStdString()) + point_offset;
                QPointF dst = cmpos(step.right(2).toStdString()) +point_offset;

                if(b==0){
                    painter->setPen(QPen(QBrush(Qt::red), linewidth, Qt::SolidLine));
                    b=1;
                }
                else{
                    painter->setPen(QPen(QBrush(Qt::green), linewidth, Qt::SolidLine));
                    b=0;
                }

                if(cb==this){ //draw on chessboard

                    // draw move line
                    painter->drawLine(src, dst);

                    //draw move step num
                    painter->setPen(QPen(QBrush(Qt::white), linewidth, Qt::SolidLine));
                    painter->drawText(src+QPoint(-5,15), QString::number(n));
                }
                else if(cmpos(step.toStdString())==this->pos()) { //draw on chessman
                    painter->setPen(QPen(QBrush(Qt::white), linewidth, Qt::SolidLine));
                    painter->drawText(20,40, QString::number(n));
                }

                n++;
            }
        }
    }
}

void ChessGraphic::setSelect(ChessGraphic *select)
{
    this->select = select;
}
void ChessGraphic::setChessBoard(ChessGraphic *chessboard)
{
    this->cb = chessboard;
}
void ChessGraphic::setInitPos(QPointF initPos)
{
    this->initPos = initPos;
}




/* ChessMan */

ChessMan::ChessMan(const QString fname, int x, int y)
{
    init(fname,x,y);
}

void ChessMan::init(QString fname, int x, int y)
{
    this->x = x;
    this->y = y;
    this->fname = fname;

    QImageReader reader;
    reader.setFileName("res/images/"+fname);
    image = reader.read();

    setZValue((x + y) % 2);

    setFlags(ItemIsSelectable);
    //setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptsHoverEvents(true);
}


QRectF ChessMan::boundingRect() const
{
    return QRectF(0, 0, 57, 57);
}

QPainterPath ChessMan::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, 57, 57);
    return path;
}

void ChessMan::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QRectF target(0.0, 0.0, 57.0, 57.0);
    painter->drawImage(target, image);

    if(cb !=NULL && cb->lastMove != NULL && cb->lastMove == this){
        //painter->setBrush( Qt::green );
        painter->drawEllipse( 0, 0, gw, gw);
    }

    drawEngineThink(painter);
}

void ChessMan::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(cb !=NULL && cb->hover!= NULL){
        cb->hover->setPos(this->pos());
        cb->update();
    }
}

void ChessMan::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();

    if(cb->bPcTurn){
        return;
    }

    if(cb->nCheckedBy==0 || cb->cgame->CheckedBy()!=cb->nCheckedBy) {
        cb->bCheckedBy = false;
    }


    QPoint dstPos = pos_int(this->pos());
    QString fen = cb->cgame->get_fen();
    QString move = cb->cgame->toIccsStr( cb->srcPos.x(), cb->srcPos.y(), dstPos.x(), dstPos.y() ).c_str();
    move += QChar(cb->cms->indexOf(cb->holdcm));

    if(ishover){
        log("hover pressed ");

        if(  !cb->cgame->is_validate( cb->srcPos.x(), cb->srcPos.y(), dstPos.x(), dstPos.y()) ){
            return;
        }

        if(cb->holdcm !=NULL && tryMove(dstPos)){
            // move cm only
            cb->backward_steps << fen + " " + move;
            think();
        }
    }
    else{
        log("cm pressed ");
        if(cb->select != NULL){
            // select src cm
            if(cb->holdcm == NULL ||
                    !cb->cgame->is_validate( cb->srcPos.x(), cb->srcPos.y(), dstPos.x(), dstPos.y()) ){

                cb->srcPos = dstPos;
                cb->holdcm = this;
                cb->select->setPos(this->pos());
            }
            else if(tryMove(dstPos)){
                // user eat cm
                cb->backward_steps << fen +" "+ move + QChar(cb->cms->indexOf(this));
                setPos(-70,0);
                cb->cgame->to_Fen();
                think();
            }
        }
    }

    if (cb->cgame->is_mate()) {
        cb->bPcMate = true;
        log("pc is mate!");
    }
    cb->update();

}

void ChessMan::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        stuff << event->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void ChessMan::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}
