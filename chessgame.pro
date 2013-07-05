QT       += core gui opengl

INCLUDEPATH  += inc eleeye

HEADERS += \
    log.h \
    define.h \
    inc/chessgame.h

SOURCES += \
    log.cpp \
    chessgame.cpp \
    src/ChessGame.cpp \

LIBS += -L/usr/lib -L. -leleeye \
        -lPocoUtil \
        -lPocoFoundation \
        -lPocoJSON \
        -lPocoXML
