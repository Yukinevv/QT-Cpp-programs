#include "mainwindow.h"
#include "ekran.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Ekran screen;
    screen.show();

    return a.exec();
}
