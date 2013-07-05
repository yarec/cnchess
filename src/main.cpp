#include "pocolog.h"

#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QImageReader>

#include "ChessMan.h"
#include "ChessBoard.h"
#include "view.h"

#include "qdebug.h"
#include "mainwindow.h"

#include <iostream>

void usage()
{
    qWarning() << "Usage: mainwindow [-SizeHint<color> <width>x<height>] ...";
    exit(1);
}
QMap<QString, QSize> parseCustomSizeHints(int argc, char **argv)
{
    QMap<QString, QSize> result;

    for (int i = 1; i < argc; ++i) {
        QString arg = QString::fromLocal8Bit(argv[i]);

        if (arg.startsWith(QLatin1String("-SizeHint"))) {
            QString name = arg.mid(9);
            if (name.isEmpty())
                usage();
            if (++i == argc)
                usage();
            QString sizeStr = QString::fromLocal8Bit(argv[i]);
            int idx = sizeStr.indexOf(QLatin1Char('x'));
            if (idx == -1)
                usage();
            bool ok;
            int w = sizeStr.left(idx).toInt(&ok);
            if (!ok)
                usage();
            int h = sizeStr.mid(idx + 1).toInt(&ok);
            if (!ok)
                usage();
            result[name] = QSize(w, h);
        }
    }

    return result;
}

int main(int argc, char *argv[])
{
    QApplication app (argc, argv);


    initlogger();

//    Q_INIT_RESOURCE(chessboard);

     QMap<QString, QSize> customSizeHints = parseCustomSizeHints(argc, argv);
     MainWindow mainWin(customSizeHints);
     mainWin.setWindowIcon(QPixmap(":/images/qt.png"));
     mainWin.setWindowTitle(QString::fromUtf8("象棋"));
     mainWin.move(200,50);
     mainWin.show();
     return app.exec();
}
