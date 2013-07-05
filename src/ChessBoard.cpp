#include "pocolog.h"
#include "ChessBoard.h"
#include "define.h"
#include <QtGui>
#include <iostream>

ChessBoard::ChessBoard( int x, int y)
{
    this->x = x;
    this->y = y;

    setZValue(-1);

    setFlags(ItemIsSelectable );
    setAcceptsHoverEvents(true);

    bPcTurn = false;
    cgame = NULL;
    newGame();
    holdcm = NULL;
    lastMove = NULL;
    nCheckedBy = 0;

}
ChessBoard::~ChessBoard() {
    if(cgame != NULL) {
         delete cgame;
         cgame = NULL;
    }
    log("~ chessboard");
}

void ChessBoard::initScene(QGraphicsScene &scene)
{
    log("start initScene... %d ", 1);
    QImageReader reader;
    reader.setFileName("res/images/bg.png");
    QImage image = reader.read();
    this->imgbg= image;
    cms = new QList<ChessGraphic*>();
    setChessBoard(this);

    scene.addItem((QGraphicsItem *)this);

    select = new ChessMan("select.png", 0, 0);
    select ->setPos(QPointF(0, 0));
    scene.addItem(select);
    setSelect(select);

    hover = new ChessMan("select.png", 0, 0);
    hover->asHover();
    hover->setChessBoard(this);
    hover->setPos(QPointF(0, 0));
    scene.addItem(hover);


    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 9; x++) {
            if(InitChessBoard[y][x]!=NOCHESS) {
                QString imgfname= (chessmannames[InitChessBoard[y][x]]+".png").c_str();
                ChessMan *item = new ChessMan(imgfname, 0, 0);
                item->setChessBoard(this);
                item->setPos(cmpos(x, y));
                item->setInitPos(cmpos(x, y));
                //item->setOpacity(0.8);
                item->show();
                scene.addItem(item);
                *cms << item;
            }
        }
    }
    log("initScene ok!");

    update();
}

void ChessBoard::start(){
//    while(true){
//        cgame->think();
//        moveCM(cgame->get_bestmove());
//        if(cgame->is_mate()){
//            log("is mate!");
//            break;
//        }
//        if(cgame->is_nobestmove()){
//            log("nobestmove!");
//            break;
//        }
//    }
    log("game over!");
}

void ChessBoard::newGame()
{
    if(bPcTurn) {
//        QMessageBox::StandardButton reply;
//        reply = QMessageBox::information(this, QObject::tr("forward"), QString::fromUtf8("pc is thinking!"));
    }
    else {
        if(cgame != NULL) {
            delete cgame;
            cgame = NULL;
        }

        cgame = new ChessGame;
        cur_step_num = 0;
//        steps.clear();
//        initScene();
    }
}

void ChessBoard::backward() {

    if(!backward_steps.isEmpty()){
        QString     step_str =backward_steps.takeLast();
        forward_steps << step_str;

        log(step_str.toStdString());

        QStringList step_split = step_str.split(" ");

        //move back pc move
        if(step_split.length()==4){
            moveBackCm(step_split[3]);
        }

        //move back user move
        moveBackCm(step_split[2]);

        cgame->set_fen(step_split[0].toLatin1().data());

        bCheckedBy = false;
        bPcMate= false;
        bMate= false;
    }
}

void ChessBoard::forward() {

    if(!forward_steps.isEmpty()){
    }

    /*
    if(tmpbacksteps.size() > 0) {
        QString s1 = "";
        s1 += tmpbacksteps.last();
        steps << s1;
        cur_step_num++;
        QStringList step_fen_moves = s1.split(",",QString::SkipEmptyParts);
        while( step_fen_moves.size()>0 ) {
            if( step_fen_moves.size() == 1 ) {
                cgame->set_fen(step_fen_moves.last().split(" ").first().toAscii().data());
            }
            else {
                movePiece(step_fen_moves.last().left(4).toAscii().data());
                if(step_fen_moves.last().length()>4) {
                    int n = step_fen_moves.last().mid(4,1).toAscii().at(0)-33;
                    chessman[n]->move(-100, -100);
                    chessman[n]->setHidden(false);
                    chessman[n]->setDisabled(false);
                }
            }
            step_fen_moves.removeLast();
        }
        tmpbacksteps.removeLast();
    }
    */
//    QMessageBox::StandardButton reply;
//    reply = QMessageBox::information(this, tr("forward"), QString::fromUtf8("这是什么"));
}


QRectF ChessBoard::boundingRect() const
{
    return QRectF(0, 0, 521, 577);
}

QPainterPath ChessBoard::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, 521, 577);
    return path;
}

void ChessBoard::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    painter->drawImage(boundingRect(), imgbg);

    // draw a-i, 0-9
    QFont font("Arial",16, QFont::Normal ,true);
    painter->setFont(font);

    for(int i=0; i<9; i++){
        painter->drawText(lw+gw*i,16, QChar('a'+i));
        painter->drawText(lw+gw*i,572, QChar('a'+i));
    }
    for(int i=0; i<=9; i++){
        painter->drawText(6,th+gw*i, QChar('9'-i));
    }

    // draw 输/赢/将军
    font = QFont("Arial",160, QFont::Bold,true);
    painter->setFont(font);
    if(bMate){
        painter->drawText(150,315,QString::fromUtf8("输!"));
    } else if(bPcMate){
        painter->drawText(150,315,QString::fromUtf8("赢!"));
    }else if(bCheckedBy){
        font = QFont("Arial",80, QFont::Bold,true);
        painter->setFont(font);
        painter->setPen(QColor(Qt::yellow));
        painter->drawText(150,315,QString::fromUtf8("将军!"));
    }

    drawEngineThink(painter);
}

void ChessBoard::hoverMoveEvent(QGraphicsSceneHoverEvent *event){
    if(hover!=NULL){
        hover->setPos(cmpos(event->pos()));
    }
}

void ChessBoard::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    QPoint dstPos = pos_int(event->pos());

    if( bPcTurn ){
        log("pc is thinking!");
        return;
    }
    else if( !cgame->is_validate(srcPos.x(), srcPos.y(), dstPos.x(), dstPos.y()) ){
        log("not validate mv");
        return;
    }

    QGraphicsItem::mousePressEvent(event);
    update();
}

void ChessBoard::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        stuff << event->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void ChessBoard::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}
