#include "ekran.h"
#include "ui_ekran.h"

#include <QPainter>
#include <QFileDialog>

Ekran::Ekran(QWidget *parent)
            : QWidget{parent},
              img(500, 500, QImage::Format_RGB32),
              img2(500, 500, QImage::Format_RGB32),
              img3(500, 500, QImage::Format_RGB32),
              ui(new Ui::Ekran)
{
    ui->setupUi(this);
}

Ekran::~Ekran()
{
    delete ui;
}

void Ekran::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(0, 0, img);
}

void Ekran::drawPixel(int x, int y, int r, int g, int b)
{
    if(x < 0 || x >= width || y < 0 || y >= height)
        return;

    uchar *pix = img.scanLine(y);
    pix[4*x] = b;
    pix[4*x + 1] = g;
    pix[4*x + 2] = r;
    update();
}

bool Ekran::pixelWhiteOrBlack(int x, int y)
{
    uchar *pix = img2.scanLine(y);

    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        if (pix[4*x] > 0)
            return true; // bialy
        else
            return false; // czarny
    }
    return false;
}

// zamiana obrazka na czarno-bialy
void Ekran::pictureTranslate()
{
    uchar *pix;
    pix = img.bits();

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if (pix[width*4*y + 4*x] + pix[width*4*y + 4*x + 1] + pix[width*4*y + 4*x + 2] >= 382)
            {
                drawPixel(x, y, 255, 255, 255);
            }
            else
            {
                drawPixel(x, y, 0, 0, 0);
            }
        }
    }
}

void Ekran::erosion()
{
    bool flag = false;
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            for (int i = x - 1; i <= x + 1; i++)
            {
                for (int j = y - 1; j <= y + 1; j++)
                {
                    if (y != i && x != j && i >= 0 && i < width && j >= 0 && j < height)
                    {
                        if (pixelWhiteOrBlack(i, j) == true) // jezeli kolor piksela jest bialy
                        {
                            flag = true;
                        }
                    }
                }
            }
            if (flag == true)
            {
                drawPixel(x, y, 255, 255, 255);
                flag = false;
            }
        }
    }
    img2 = img;
    update();
}

void Ekran::dilation()
{
    bool flag = false;
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            for (int i = x - 1; i <= x + 1; i++)
            {
                for (int j = y - 1; j <= y + 1; j++)
                {
                    if (y != i && x != j && i >= 0 && i < width && j >= 0 && j < height)
                    {
                        if (pixelWhiteOrBlack(i, j) == false) // jezeli kolor piksela jest czarny
                        {
                            flag = true;
                        }
                    }
                }
            }
            if (flag == true)
            {
                drawPixel(x, y, 0, 0, 0);
                flag = false;
            }
        }
    }
    img2 = img;
    update();
}

void Ekran::on_pushButton_clicked()
{
    erosion();
}

void Ekran::on_pushButton_2_clicked()
{
    dilation();
}

void Ekran::on_pushButton_3_clicked()
{
    erosion();
    dilation();
}

void Ekran::on_pushButton_4_clicked()
{
    dilation();
    erosion();
}

// wczytywanie obrazka
void Ekran::on_pushButton_5_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Wybierz obraz", QDir::currentPath(),
        "Obrazy (*.png *.jpg *.jpeg *.bmp *.gif)");

    if (QString::compare(filename, QString()) != 0)
    {
        img.load(filename);
        img = img.scaled(width, height, Qt::IgnoreAspectRatio);
        pictureTranslate();
        img2 = img;
        img3 = img;
    }
}

// reset
void Ekran::on_pushButton_6_clicked()
{
    img = img3;
    img2 = img3;
    update();
}
