#include "ekran.h"
#include "ui_ekran.h"

#include <QPainter>

Ekran::Ekran(QWidget *parent)
            : QWidget(parent),
              ui(new Ui::Ekran)
{
    ui->setupUi(this);
    img = new QImage(500, 500, QImage::Format_RGB32);
    images[0] = new QImage(":/zdj1.jpg");
    images[1] = new QImage(":/zdj2.jpg");
    images[2] = new QImage(":/zdj3.jpg");

    for (int i = 0; i < 3; i++)
    {
        showPicture[i] = 1;
        hide[i] = 100; // wartosc zmieniana suwakiem, potrzebna do wyliczenia alphaR
        mode[i] = 0;
    }
    addModes();
    drawImage();
    update();
}

Ekran::~Ekran()
{
    delete ui;
}

void Ekran::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(0, 0, *img);
}

void Ekran::fillWhite(uchar *pix)
{
    for (int i = 0; i < width; i++)
    {
        pix = img->scanLine(i);

        for (int j = 0; j < width; j++)
        {
            pix[4 * j] = 255;
            pix[4 * j + 1] = 255;
            pix[4 * j + 2] = 255;
        }
    }
}

// Funkcje blendujace
int Ekran::lighten(int c1, int c2)
{
    if (c1 > c2)
        return c1;
    else
        return c2;
}

int Ekran::darken(int c1, int c2)
{
    if (c1 < c2)
        return c1;
    else
        return c2;
}

int Ekran::multiply(int c1, int c2)
{
    return (c1 * c2) >> 8;
}

int Ekran::diff(int c1, int c2)
{
    return abs(c2 - c1);
}

void Ekran::drawImage()
{
    uchar *pix1, *pix2;
    fillWhite(pix1);

    for (int k = 0; k < 3; k++)
    {
        if (showPicture[k] != 0)
        {
            for (int i = 0; i < height; i++)
            {
                pix1 = img->scanLine(i);
                pix2 = images[k]->scanLine(i);

                for (int j = 0; j < width; j++)
                {
                    double alpha = (double)hide[k] / 100.0;
                    double alphaR = 1.0 - alpha;
                    int r1 = pix1[4 * j];
                    int g1 = pix1[4 * j + 1];
                    int b1 = pix1[4 * j + 2];
                    int r2 = pix2[4 * j];
                    int g2 = pix2[4 * j + 1];
                    int b2 = pix2[4 * j + 2];

                    switch(mode[k])
                    {
                    //case 0: tryb default
                    case 1:
                    {
                        r2 = multiply(r2, r1);
                        g2 = multiply(g2, g1);
                        b2 = multiply(b2, b1);
                        break;
                    }
                    case 2:
                    {
                        r2 = darken(r2, r1);
                        g2 = darken(g2, g1);
                        b2 = darken(b2, b1);
                        break;
                    }
                    case 3:
                    {
                        r2 = lighten(r2, r1);
                        g2 = lighten(g2, g1);
                        b2 = lighten(b2, b1);
                        break;
                    }
                    case 4:
                    {
                        r2 = diff(r2, r1);
                        g2 = diff(g2, g1);
                        b2 = diff(b2, b1);
                        break;
                    }
                    }
                    pix1[4 * j] = alpha * r2 + alphaR * pix1[4 * j];
                    pix1[4 * j + 1] = alpha * g2 + alphaR * pix1[4 * j + 1];
                    pix1[4 * j + 2] = alpha * b2 + alphaR * pix1[4 * j + 2];
                }
            }
        }
    }
    update();
}

void Ekran::on_comboBox_3_currentIndexChanged(int index)
{
    mode[0] = index;
    drawImage();
}

void Ekran::on_comboBox_2_currentIndexChanged(int index)
{
    mode[1] = index;
    drawImage();
}

void Ekran::on_comboBox_currentIndexChanged(int index)
{
    mode[2] = index;
    drawImage();
}

void Ekran::on_checkBox_3_stateChanged(int arg1)
{
    showPicture[0] = arg1;
    drawImage();
}

void Ekran::on_checkBox_2_stateChanged(int arg1)
{
    showPicture[1] = arg1;
    drawImage();
}

void Ekran::on_checkBox_stateChanged(int arg1)
{
    showPicture[2] = arg1;
    drawImage();
}

void Ekran::on_horizontalSlider_3_valueChanged(int value)
{
    hide[0] = value;
    drawImage();
}

void Ekran::on_horizontalSlider_2_valueChanged(int value)
{
    hide[1] = value;
    drawImage();
}

void Ekran::on_horizontalSlider_valueChanged(int value)
{
    hide[2] = value;
    drawImage();
}

void Ekran::addModes()
{
    ui->comboBox->addItem("Default", 1);
    ui->comboBox->addItem("Multiply", 2);
    ui->comboBox->addItem("Darken", 3);
    ui->comboBox->addItem("Lighten", 4);
    ui->comboBox->addItem("Diff", 5);

    ui->comboBox_2->addItem("Default", 1);
    ui->comboBox_2->addItem("Multiply", 2);
    ui->comboBox_2->addItem("Darken", 3);
    ui->comboBox_2->addItem("Lighten", 4);
    ui->comboBox_2->addItem("Diff", 5);

    ui->comboBox_3->addItem("Default", 1);
    ui->comboBox_3->addItem("Multiply", 2);
    ui->comboBox_3->addItem("Darken", 3);
    ui->comboBox_3->addItem("Lighten", 4);
    ui->comboBox_3->addItem("Diff", 5);
}
