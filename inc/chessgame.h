#ifndef CHESSGAME_H
#define CHESSGAME_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>


#ifdef _WIN32
  #include <windows.h>
  const char *const szFileName = "eleeye.exe";
#else
  #include <dlfcn.h>
  const char *const szFileName = "eleeyebin";
#endif
#include "base/base2.h"
#include "base/parse.h"
#include "base/pipe.h"
#include "base/wsockbas.h"
#include "codec/base64/base64.h"
#include "eleeye/position.h"
#include "cchess/cchess.h"
#include "cchess/ecco.h" 
#include "cchess/pgnfile.h" 


#include "log.h" 

#include <QApplication>
const int MAX_CHAR = 8192; // 输入报告的最大行长度，同时也是引擎发送和接收信息的最大行长度

const int BESTMOVE_THINKING = 0; // 引擎正在思考，没有反馈值
const int BESTMOVE_DRAW = -1;    // 引擎接受提和的反馈值
const int BESTMOVE_RESIGN = -2;  // 引擎认输的反馈值
const int BESTMOVE_TIMEOUT = -3; // 引擎超时的反馈值
const char *const cszGo = "go time %d increment %d opptime %d oppincrement %d";
const char *const cszGoNodes = "go nodes %d";
const char *const cszGoDraw = "go draw time %d increment %d opptime %d oppincrement %d";

class ChessGame {
    public:
        ChessGame();
        ~ChessGame();

    private:
        char szLineStr[MAX_CHAR];
        char szFen[MAX_CHAR];
        char szFenTmp[MAX_CHAR];
        char szMoves[MAX_CHAR];
        char szDepth[MAX_CHAR];
        bool bStarted, bUseMilliSec, bDraw, bnobestmove;
        int64_t llTime ;
        int nStatus;

        PipeStruct _pipe;
        PositionStruct pos;
        uint32_t dwMoveStr;
        uint32_t dwLastMove;
        std::string bestmove;


        void Send(const char *szLineStr);
        bool Receive(char *szLineStr);
    public:

        void start_engine();
        void stop_engine(); 

        void think(int M, int ms);

        void think(int ms);

        bool make_move(int mv);

        bool make_move(char * mv);

        bool make_move(int x, int y, int dx, int dy);

        char* get_depth();

        char* get_fen();

        void set_fen(char* szfen);

        void to_Fen();

        void to_Fen(char* fen);

        std::string get_lastmv(int i); 

        std::string get_lastmv();

        std::string get_bestmove();

        std::string toIccsStr(int x, int y, int dx, int dy);

        int toIccs(int x, int y, int dx, int dy);

        bool is_validate (int mv);

        bool is_validate (int x, int y, int dx, int dy);

        bool is_mate();

        bool is_nobestmove();

        int CheckedBy();

        void Rollback();
};
#endif
