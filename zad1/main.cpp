#include "mainwindow.h"

#include <QApplication>
#include "ekran.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    Ekran w;
    w.show();
    return a.exec();
}
