QT       += core gui opengl

INCLUDEPATH  += inc eleeye

HEADERS += \
    inc/log.h \
    inc/define.h \
    inc/ChessMan.h \
    inc/ChessBoard.h \
    inc/view.h \
    inc/chessgame.h

SOURCES += \
    src/ChessGame.cpp \
    src/view.cpp \
    src/main.cpp \
    src/log.cpp \
    src/ChessMan.cpp \
    src/ChessBoard.cpp

LIBS += -L/usr/lib -L. -leleeye \
        -lPocoUtil \
        -lPocoFoundation \
        -lPocoJSON \
        -lPocoXML
