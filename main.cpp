#include "mainwindow.h"
#include "graphview.h"
#include <QApplication>
#include "graph.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
