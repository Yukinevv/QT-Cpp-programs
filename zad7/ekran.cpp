#include "ekran.h"
#include "ui_ekran.h"

#include <QMouseEvent>
#include <QPainter>

Ekran::Ekran(QWidget *parent)
    : QWidget{parent},
      im(512, 512, QImage::Format_RGB32),
      tmp(512, 512, QImage::Format_RGB32),
      ui(new Ui::Ekran)
{
    ui->setupUi(this);
    im.fill(0);

    ui->sliderRed->setMaximum(255);
    ui->sliderGreen->setMaximum(255);
    ui->sliderBlue->setMaximum(255);
    ui->sliderH->setMaximum(360);
    ui->sliderS->setMaximum(100);
    ui->sliderV->setMaximum(100);
    moveSliders();
}

Ekran::~Ekran()
{
    delete ui;
}

void Ekran::drawPixel(int x, int y, int r, int g, int b)
{
    if(x < 0 || x >= im.width() || y < 0 || y >= im.height())
        return;

    uchar *pix = im.scanLine(y);
    pix[4*x] = b;
    pix[4*x + 1] = g;
    pix[4*x + 2] = r;
    update();
}

void Ekran::drawSquareRGB(char whichColor)
{
    int n = 0, m = 0;
    if(whichColor == 'R')
    {
        for(int i = 0; i < im.height(); i+=2)
        {
            for(int j = im.width() - 1; j > 0; j-=2)
            {
                drawPixel(i, j, kR, m, n);
                drawPixel(i+1, j, kR, m, n);
                drawPixel(i, j-1, kR, m, n);
                drawPixel(i+1, j+1, kR, m, n);
                n++;
            }
            n = 0;
            m++;
        }
    }
    else if(whichColor == 'G')
    {
        for(int i = 0; i < im.height(); i+=2)
        {
            for(int j = im.width() - 1; j > 0; j-=2)
            {
                drawPixel(i, j, m, kG, n);
                drawPixel(i+1, j, m, kG, n);
                drawPixel(i, j-1, m, kG, n);
                drawPixel(i+1, j+1, m, kG, n);
                n++;
            }
            n = 0;
            m++;
        }
    }
    else if(whichColor == 'B')
    {
        for(int i = 0; i < im.height(); i+=2)
        {
            for(int j = im.width() - 1; j > 0; j-=2)
            {
                drawPixel(i, j, m, n, kB);
                drawPixel(i+1, j, m, n, kB);
                drawPixel(i, j-1, m, n, kB);
                drawPixel(i+1, j+1, m, n, kB);
                n++;
            }
            n = 0;
            m++;
        }
    }
    update();
}

void Ekran::drawSquareHSV(char whichSlider)
{
    float n = 0, m = 0;
    if(whichSlider == 'H')
    {
        n = 1.0 / (float)im.width();
        m = 1.0 / (float)im.width();
        float lH = kH, lS = 0, lV = 0;
        for(int i = 0; i < im.height(); i++)
        {
            for(int j = im.width() - 1; j > 0; j--)
            {
                HSVToRGB(lH, lS*100, lV*100, true);
                drawPixel(i, j, rysR, rysG, rysB);
                lS += n;
            }
            lS = 0;
            lV += m;
        }
    }
    else if(whichSlider == 'S')
    {
        n = 1.0 / (float)im.width();
        m = 360.0 / (float)im.width();
        float lH = 0, lS = kS, lV = 0;
        for(int i = im.height() - 1; i >= 0; i--)
        {
            for(int j = im.width() - 1; j >= 0; j--)
            {
                HSVToRGB(lH, lS, lV*100, true);
                drawPixel(i, j, rysR, rysG, rysB);
                lV += n;
            }
            lV = 0;
            lH += m;
        }
    }
    else if(whichSlider == 'V')
    {
        n = 1.0 / (float)im.width();
        m = 360.0 / (float)im.width();
        float lH = 0, lS = 0, lV = kV;
        for(int i = im.height() - 1; i >= 0; i--)
        {
            for(int j = im.width() - 1; j >= 0; j--)
            {
                HSVToRGB(lH, lS*100, lV, true);
                drawPixel(i, j, rysR, rysG, rysB);
                lS += n;
            }
            lS = 0;
            lH += m;
        }
    }
    update();
}

void Ekran::HSVToRGB(float H, float S, float V, bool local)
{
    float lS = S / 100, lV = V / 100, C = lS * lV;
    float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
    float m = lV - C, lR, lG, lB;

    if(H >= 0 && H < 60)
    {
        lR = C, lG = X, lB = 0;
    }
    else if(H >= 60 && H < 120)
    {
        lR = X, lG = C, lB = 0;
    }
    else if(H >= 120 && H < 180)
    {
        lR = 0, lG = C, lB = X;
    }
    else if(H >= 180 && H < 240)
    {
        lR = 0, lG = X, lB = C;
    }
    else if(H >= 240 && H < 300)
    {
        lR = X, lG = 0, lB = C;
    }
    else
    {
        lR = C, lG = 0, lB = X;
    }

    if(local == true)
    {
        rysR = (lR + m) * 255;
        rysG = (lG + m) * 255;
        rysB = (lB + m) * 255;
    }
    else
    {
        kR = (lR + m) * 255;
        kG = (lG + m) * 255;
        kB = (lB + m) * 255;
    }
}

float Ekran::maxValue(float x, float y, float z)
{
    if(x > y)
    {
        if(x > z)
            return x;
        else
            return z;
    }
    else
    {
        if(y > z)
            return y;
        else
            return z;
    }
}

float Ekran::minValue(float x, float y, float z)
{
    if(x < y)
    {
        if(x < z)
            return x;
        else
            return z;
    }
    else
    {
        if(y < z)
            return y;
        else
            return z;
    }
}

void Ekran::RGBToHSV(float R, float G, float B)
{
    R /= 255, G /= 255, B /= 255;
    float M = maxValue(R, G, B);
    float m = minValue(R, G, B);
    float C = M - m;

    if(M == m)
    {
        kH = 0;
    }
    else if(M == R)
    {
        kH = fmod((60 * ((G - B) / C) + 360), 360.0);
    }
    else if(M == G)
    {
        kH = fmod((60 * ((B - R) / C) + 120), 360.0);
    }
    else if(M == B)
    {
        kH = fmod((60 * ((R - B) / C) + 240), 360.0);
    }

    if(M == 0)
    {
        kS = 0;
    }
    else
    {
        kS = (C / M * 100.0);
    }
    kV = M * 100.0;
}

//Events
void Ekran::mousePressEvent(QMouseEvent *e)
{
    m1 = e->pos();
    outside_the_area = false;

    if(m1.x() >= 0 && m1.x() < im.width() && m1.y() >= 0 && m1.y() < im.height())
        qDebug("x1: %d   y1: %d", m1.x(), m1.y());
    else
    {
        qDebug("Rysujesz poza obszarem");
        outside_the_area = true;
    }

    if(m1.x() < 0 || m1.x() >= im.width() || m1.y() < 0 || m1.y() >= im.height())
        return;

    tmp = im;
}

void Ekran::mouseMoveEvent(QMouseEvent *e)
{
    m2 = e->pos();
    im = tmp;

    if(m1.x() < 0 || m1.x() >= im.width() || m1.y() < 0 || m1.y() >= im.height())
        return;
}

void Ekran::mouseReleaseEvent(QMouseEvent *e)
{
    m2 = e->pos();

    if(m2.x() >= 0 && m2.x() < im.width() && m2.y() >= 0 && m2.y() < im.height() && outside_the_area == false)
        qDebug("x2: %d   y2: %d", m2.x(), m2.y());
    else if(outside_the_area == true)
        return;
    else
        qDebug("Rysujesz poza obszarem");
}

void Ekran::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(0, 0, width(), height()/2, Qt::white);
    p.fillRect(0, height()/2, width(), height()/2, Qt::white);

    p.drawImage(0, 0, im);
}

//Metody z UI
void Ekran::on_quitButton_clicked()
{
    qApp->quit();
}

void Ekran::on_sliderRed_sliderMoved(int position)
{
    kR = position;
    QString qstr = QString::number(position);
    ui->labelRedValue->setText(qstr);
    drawSquareRGB('R');
    RGBToHSV(kR, kG, kB);
    moveSliders();
}

void Ekran::on_sliderGreen_sliderMoved(int position)
{
    kG = position;
    QString qstr = QString::number(position);
    ui->labelGreenValue->setText(qstr);
    drawSquareRGB('G');
    RGBToHSV(kR, kG, kB);
    moveSliders();
}

void Ekran::on_sliderBlue_sliderMoved(int position)
{
    kB = position;
    QString qstr = QString::number(position);
    ui->labelBlueValue->setText(qstr);
    drawSquareRGB('B');
    RGBToHSV(kR, kG, kB);
    moveSliders();
}

void Ekran::on_sliderH_sliderMoved(int position)
{
    kH = position;
    QString qstr = QString::number(position);
    ui->labelHValue->setText(qstr);
    drawSquareHSV('H');
    HSVToRGB(kH, kS, kV, false);
    moveSliders();
}

void Ekran::on_sliderS_sliderMoved(int position)
{
    kS = position;
    QString qstr = QString::number(position);
    ui->labelSValue->setText(qstr);
    drawSquareHSV('S');
    HSVToRGB(kH, kS, kV, false);
    moveSliders();
}

void Ekran::on_sliderV_sliderMoved(int position)
{
    kV = position;
    QString qstr = QString::number(position);
    ui->labelVValue->setText(qstr);
    drawSquareHSV('V');
    HSVToRGB(kH, kS, kV, false);
    moveSliders();
}

void Ekran::moveSliders()
{
    ui->sliderRed->setValue(kR);
    QString qstr = QString::number(ui->sliderRed->value());
    ui->labelRedValue->setText(qstr);

    ui->sliderGreen->setValue(kG);
    qstr = QString::number(ui->sliderGreen->value());
    ui->labelGreenValue->setText(qstr);

    ui->sliderBlue->setValue(kB);
    qstr = QString::number(ui->sliderBlue->value());
    ui->labelBlueValue->setText(qstr);

    ui->sliderH->setValue(kH);
    qstr = QString::number(ui->sliderH->value());
    ui->labelHValue->setText(qstr);

    ui->sliderS->setValue(kS);
    qstr = QString::number(ui->sliderS->value());
    ui->labelSValue->setText(qstr);

    ui->sliderV->setValue(kV);
    qstr = QString::number(ui->sliderV->value());
    ui->labelVValue->setText(qstr);
}
