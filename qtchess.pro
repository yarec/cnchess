QT           += core gui opengl
TEMPLATE      = app
INCLUDEPATH  += inc eleeye

LIBS         += -L/usr/lib -L. -leleeye \
                -llog4cpp \
                #-lPocoUtil -lPocoFoundation -lPocoJSON -lPocoXML

HEADERS      += inc/chessgame.h inc/log.h inc/pocolog.h inc/define.h \
                inc/colorswatch.h inc/mainwindow.h inc/toolbar.h \
                inc/ChessMan.h inc/ChessBoard.h inc/view.h

SOURCES      += src/colorswatch.cpp src/mainwindow.cpp src/toolbar.cpp src/main.cpp \
                src/view.cpp  src/ChessMan.cpp  src/ChessGame.cpp  src/ChessBoard.cpp  src/pocolog.cpp

RESOURCES    += res/images/chessboard.qrc res/images/mainwindow.qrc
DEFINES      += QTC_WCHAR_FIX  LOG4CPP #POCOLOG 
CONFIG       += debug #release static

DESTDIR      = . #bin
OBJECTS_DIR  = obj
RC_FILE      = res/images/myapp.rc

dist         = dist
# install
target.path  = $$dist
res.files    = res/book.dat res/evaluate.dll
res.path     = $$dist
wav.files    = res/*.wav
wav.path     = $$dist/res
eleeye.files = eleeye.exe
eleeye.path  = $$dist
qt.files     = d:/Qt/2009.02/qt/bin/QtGui4.dll d:/Qt/2009.02/qt/bin/QtCore4.dll
qt.path      = $$dist
INSTALLS     += target res wav eleeye qt

