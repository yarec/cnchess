
#include "chessgame.h"

#include <iostream>
#include <sstream>

  using namespace std;

ChessGame::ChessGame() {
    fflush(stdout);
    PreGenInit();
    ChineseInit();
    pos.FromFen(cszStartFen);
    pos.ToFen(szFen);
    bStarted = false;
    bnobestmove = false;
    bestmove = "";
    szMoves[0]='\0';
    //for(int i=0; i<MAX_CHAR; i++) szMoves[i]='\0';
    start_engine();
}

ChessGame::~ChessGame() {
    stop_engine();
}

void ChessGame::Send(const char *szLineStr) {
    _pipe.LineOutput(szLineStr);
    RLOG("send : %s\n",szLineStr);
}

bool ChessGame::Receive(char *szLineStr) {
    if (_pipe.LineInput(szLineStr)) {
        RLOG("receive ok: %s\n",szLineStr);
        return true;
    } else {
        //RLOG("receive non: %s\n",szLineStr);
        return false;
    }
}

void ChessGame::start_engine() {

    if (!bStarted) {
        // 如果引擎尚未启动，启动引擎
        llTime = GetTime();

        struct stat buf;
        if (stat(szFileName, &buf) == -1) {
            RLOG("Error: engine '%s' not found!\n",szFileName);
            return;
        }

        RLOG("start engine %s!\n",szFileName);

        _pipe.Open(szFileName);
        Send("ucci");
        // 发送"ucci"指令后，在10秒钟内等待"ucciok"反馈信息
        while ((int) (GetTime() - llTime) < 10000) {
            if (Receive(szLineStr)) {
                if (StrEqv(szLineStr, "option usemillisec ")) {
                    bUseMilliSec = true;
                }
                if (StrEqv(szLineStr, "ucciok")) {
                    break;
                }
            } else {
                Idle();
            }
        }
        // 设置必要的初始化选项
        //Send("setoption promotion false");
        Send("setoption ponder true");
        //Send("setoption randomness huge");
        Send("setoption newgame");
        //Send("setoption usemillisec true");

        bStarted = true;

        pos.FromFen(cszStartFen);
    }
}

void ChessGame::stop_engine () {
    llTime = GetTime();
    Send("quit");;
    while ((int) (GetTime() - llTime) < 1000) {
        if (Receive(szLineStr)) {
            if (StrEqv(szLineStr, "bye")) {
                RLOG("engine say: bye");
                break;
            }
        } else {
            Idle();
        }
    }
    _pipe.Close();
}


void ChessGame::think(int M, int ms) {

    if(!this->bStarted) {
        RLOG("\n***** Engine not start! *****: \n");
        return;
    }

    char szLineStr[MAX_CHAR];
    int mv=0, mv_ponder=0;
    char *lp;
    char *lpLineChar;
    RLOG("\n\n***** think *****: \n");

    // 向引擎发送当前局面
    llTime = GetTime();
    lpLineChar = szLineStr;
    lpLineChar += sprintf(lpLineChar, "position fen %s - - 0 1", szFen);
    RLOG("\nmoves : %s\n", szMoves);
    lpLineChar += sprintf(lpLineChar, " moves %s", szMoves);
    Send(szLineStr);


    if(M== 0) {
        sprintf(szLineStr, bDraw ? cszGoDraw : cszGo, ms, 5 * 1000, 10*1000, 5*1000);
    }
    else {
        sprintf(szLineStr, cszGoNodes, M*1000000);
    }

    Send(szLineStr);

    // 首先读取引擎反馈信息
    mv = BESTMOVE_THINKING;

    llTime = GetTime();
    while ((int) (GetTime() - llTime) < 50000) {
        if(Receive(szLineStr)) {
            lp = szLineStr;
            if (StrEqvSkip(lp, "bestmove ")) {
                mv = COORD_MOVE(*(uint32_t *) lp);

                std::stringstream s;
                s << lp[0] << lp[1] << lp[2] <<lp[3];
                s >> bestmove;

                std::cout << "bestmove : " << bestmove << std::endl;
                lp += 4;
                if (StrScan(lp, " resign")) {
                    mv = BESTMOVE_RESIGN;

                    std::cout << "resign mv: " << mv << std::endl;
                } else {
                    if (StrScan(lp, " draw")) {
                        if (bDraw) {
                            mv = BESTMOVE_DRAW;
                            std::cout << "draw mv: " << mv << std::endl;
                        } else {
                            bDraw = true;
                        }
                    } else {
                        if (StrEqvSkip(lp, " ponder ")) {
                            mv_ponder = COORD_MOVE(*(uint32_t *) lp);
                        }
                        bDraw = false;
                    };
                };
                break;
            }

            if(StrScan(lp, "nobestmove")){
                bnobestmove = true;
                RLOG("set nobestnome true");
            }

            if (StrEqvSkip(lp, "info depth ")) {
//FIXME 
                strncpy(szDepth, szLineStr, strlen(szLineStr));
            }
        } else {
            Idle();
        }

        qApp->processEvents();
    }
    //RLOG("szFen: %d %c %s\n", strlen(szFen), szFen[strlen(szFen)-1], szFen);
    //RLOG("use time: %d\n",(int)(GetTime() - llTime));

    /*
       if(!pos.LegalMove(mv)) {
       RLOG("mv legal try mv_ponder now!\n");
       mv = mv_ponder;
       }
       */

    if(mv > 0){
        if(make_move(mv)) {
            RLOG("pc move ok!\n");
        }
        else {
            RLOG("pc move false and think again! \n");
            //think(inc+1);
        }
    }
    else{
        bnobestmove = true;
    }
}

void ChessGame::think(int ms) {
    think(0, ms);
}

bool ChessGame::make_move(int mv) {
    int st = 0;
    bool ret = TryMove(pos, st, mv);

    dwMoveStr = MOVE_COORD(mv);

    char tmp[5];
    for(int i=0; i<4; i++) tmp[i]=((char*)&dwMoveStr)[i];
    tmp[4]='\0';

    strncat(szMoves,tmp,4);
    strncat(szMoves," ",1);

    return ret;
}

bool ChessGame::make_move(char * mv) {
    char* str = mv;
    uint32_t m = str[0] + (str[1] << 8) + (str[2] << 16) + (str[3] << 24);
    return make_move(COORD_MOVE(m));
}

bool ChessGame::make_move(int x, int y, int dx, int dy)
{
    return make_move(COORD_MOVE(toIccs(x,y,dx,dy)));
}

char* ChessGame::get_depth() {
    return szDepth;
}

char* ChessGame::get_fen() {
    pos.ToFen(szFenTmp);
    return szFenTmp;
}

void ChessGame::set_fen(char* szfen) {
    szMoves[0]='\0';
    strncpy(szFen, szfen, strlen(szfen));

    pos.FromFen(szFen);
}

void ChessGame::to_Fen() {
    pos.ToFen(szFen);
    szMoves[0]='\0';
}

void ChessGame::to_Fen(char* fen) {
    pos.ToFen(fen);
}

std::string ChessGame::get_bestmove() {
    if(bnobestmove){
        return NULL;
    }
    else{
        return bestmove;
    }
}

std::string ChessGame::get_lastmv(int i) {
    if (pos.nMoveNum > 1) {
        std::cout << "mvs.wmv "<< pos.rbsList[pos.nMoveNum-i].mvs.wmv << std::endl;

        dwLastMove = MOVE_COORD(pos.rbsList[pos.nMoveNum-i].mvs.wmv);
        //((char *)&dwLastMove)[4]='\0';

        std::cout << dwLastMove << endl;

        std::string tmp;
//        tmp.push_back(((char*)dwLastMove)[0]);
//        tmp.push_back(((char*)dwLastMove)[1]);
//        tmp.push_back(((char*)dwLastMove)[2]);
//        tmp.push_back(((char*)dwLastMove)[3]);
        return tmp;
    }
    else 
        return NULL; 
}

std::string ChessGame::get_lastmv() {
    return get_lastmv(1);
}

std::string ChessGame::toIccsStr(int x, int y, int dx, int dy){
    std::string tmp;
    tmp.push_back(97 + x);
    tmp.push_back(48 + 9 - y);
    tmp.push_back(97 + dx);
    tmp.push_back(48 + 9 - dy);

    return tmp;
}

int ChessGame::toIccs(int x, int y, int dx, int dy){
    int mv = 0;
    //build iccs str
    mv += (48 + 9 - dy) << 24;
    mv += (97 + dx) << 16;
    mv += (48 + 9 - y) << 8;
    mv += (97 + x) ;

    return mv;
}

bool ChessGame::is_validate (int mv) {
    return pos.LegalMove(mv);
}

bool ChessGame::is_validate (int x, int y, int dx, int dy) {
    return pos.LegalMove(COORD_MOVE(toIccs(x,y,dx,dy)));
}

bool ChessGame::is_mate() {
    return pos.IsMate();
}

bool ChessGame::is_nobestmove(){
    return bnobestmove;
}

int ChessGame::CheckedBy() {
    return pos.CheckedBy();
}

void ChessGame::Rollback() {
    pos.Rollback();
}
