#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QLayout>

MainWindow::MainWindow(QWidget *parent)
                      : QMainWindow(parent),
                        ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ekran = new Ekran();
    rezultat = new Rezultat();
    this->layout()->addWidget(ekran);
    this->layout()->addWidget(rezultat);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    update();
}

void MainWindow::barycentricCoordinates(Point p, int frame)
{
    int x = p.x, y = p.y;
    float u, v, w;

    // obliczam wspolrzedne na podstawie klatki - frame (wartosc ze slidera)
    int x_a = (1 - (float)frame / (float)frameAmount) * ekran->vertImg1[0].x + (float)frame / (float)frameAmount * ekran->vertImg2[0].x;
    int x_b = (1 - (float)frame / (float)frameAmount) * ekran->vertImg1[1].x + (float)frame / (float)frameAmount * ekran->vertImg2[1].x;
    int x_c = (1 - (float)frame / (float)frameAmount) * ekran->vertImg1[2].x + (float)frame / (float)frameAmount * ekran->vertImg2[2].x;
    int y_a = (1 - (float)frame / (float)frameAmount) * ekran->vertImg1[0].y + (float)frame / (float)frameAmount * ekran->vertImg2[0].y;
    int y_b = (1 - (float)frame / (float)frameAmount) * ekran->vertImg1[1].y + (float)frame / (float)frameAmount * ekran->vertImg2[1].y;
    int y_c = (1 - (float)frame / (float)frameAmount) * ekran->vertImg1[2].y + (float)frame / (float)frameAmount * ekran->vertImg2[2].y;

    v = (float)(((x - x_a) * (y_c - y_a) - (y - y_a) * (x_c - x_a))) /
        (float)(((x_b - x_a) * (y_c - y_a) - (y_b - y_a) * (x_c - x_a)));

    w = (float)(((x_b - x_a) * (y - y_a) - (y_b - y_a) * (x - x_a))) /
        (float)(((x_b - x_a) * (y_c - y_a) - (y_b - y_a) * (x_c - x_a)));
    u = 1 - v - w;

    int x_t, y_t, x_t2, y_t2;
    uchar *ptr, *texturePtr1, *texturePtr2;

    // jesli punkt znajduje sie wewnatrz trojkata to go zamalowuje
    if ((u > 0) && (v > 0) && (w > 0) && (u < 1) && (v < 1) && (w < 1))
    {
        // obliczam wspolrzedne (x_t, y_t) punktu P_t na teksturze
        x_t = floor(u * ekran->vertImg1[0].x + v * ekran->vertImg1[1].x + w * ekran->vertImg1[2].x);
        y_t = floor(u * ekran->vertImg1[0].y + v * ekran->vertImg1[1].y + w * ekran->vertImg1[2].y);

        x_t2 = floor(u * ekran->vertImg2[0].x + v * ekran->vertImg2[1].x + w * ekran->vertImg2[2].x);
        y_t2 = floor(u * ekran->vertImg2[0].y + v * ekran->vertImg2[1].y + w * ekran->vertImg2[2].y);

        ptr = rezultat->img->scanLine(y);
        texturePtr1 = ekran->img1->scanLine(y_t);
        texturePtr2 = ekran->img2->scanLine(y_t2);

        // koloruje punkt P wartoscia punktu P_t
        ptr[4*x] = (1 - (float)frame / (float)frameAmount) * texturePtr1[4*x_t] + (float)frame / (float)frameAmount * texturePtr2[4*x_t2];
        ptr[4*x + 1] = (1 - (float)frame / (float)frameAmount) * texturePtr1[4*x_t + 1] + (float)frame / (float)frameAmount * texturePtr2[4*x_t2 + 1];
        ptr[4*x + 2] = (1 - (float)frame / (float)frameAmount) * texturePtr1[4*x_t + 2] + (float)frame / (float)frameAmount * texturePtr2[4*x_t2 + 2];
    }
}

void MainWindow::triangleTexturing(int n)
{
    int tmpImg;
    Point p(-1, -1);

    rezultat->clearWidget();

    // usuwam krawedzie
    tmpImg = ekran->currImg;
    ekran->currImg = 0;
    ekran->clearImg();
    ekran->currImg = 1;
    ekran->clearImg();

    // dla kazdego punktu obliczam wspolrzedne barycentryczne
    for (int i = 0; i < rezultat->height(); i++)
    {
        for (int j = 0; j < rezultat->width(); j++)
        {
            barycentricCoordinates({j, i}, n);
        }
    }

    // rysuje z powrotem krawedzie
    ekran->currImg = 0;
    ekran->clearAndDrawImg(p);
    ekran->currImg = 1;
    ekran->clearAndDrawImg(p);

    ekran->currImg = tmpImg;
    update();
}

void MainWindow::on_changeImageSlider_valueChanged(int value)
{
    ekran->changeImage(value);
    update();
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    frameAmount = arg1;
    ui->changeFrameSlider->setMaximum(arg1);
}

void MainWindow::on_changeFrameSlider_valueChanged(int value)
{
    if ((ekran->pointsImg1 == 3) && (ekran->pointsImg2 == 3))
    {
        triangleTexturing(value);
    }
}

void MainWindow::on_quitButton_clicked()
{
    qApp -> quit();
}
