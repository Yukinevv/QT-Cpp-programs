#include "mainwindow.h"
#include "ekran.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget *window = new QWidget;
    window->setWindowTitle("Grafika zadania");
    window->resize(1250, 700);

    QHBoxLayout *layout = new QHBoxLayout(window);

    Ekran screen;

    layout->addWidget(&screen);

    window->show();
    return a.exec();
}
