#include "ekran.h"
#include "ui_ekran.h"

#include <QMouseEvent>
#include <QPainter>

Ekran::Ekran(QWidget *parent)
    : QWidget{parent},
      im(1200, 500, QImage::Format_RGB32),
      tmp(1200, 500, QImage::Format_RGB32),
      ui(new Ui::Ekran)
{
    ui->setupUi(this);
    im.fill(0);

    font1.setPointSize(20);
    font1.setBold(true);

    palette1 = ui->label1->palette();
    palette1.setColor(ui->label1->foregroundRole(), Qt::white);
    ui->label1->setPalette(palette1);

    ui->slider1->setMinimum(3);
    ui->slider1->setMaximum(100);

    ui->label1->setFont(font1);
    ui->label1->setText("Wartość n = 3");

    ui->labelColors->setFont(font1);
    ui->labelColors->setPalette(palette1);

    ui->colorRedButton->setStyleSheet("background-color: red;");
    ui->colorGreenButton->setStyleSheet("background-color: green;");
    ui->colorBlueButton->setStyleSheet("background-color: blue;");
    ui->colorWhiteButton->setStyleSheet("background-color: white;");
    ui->colorPurpleButton->setStyleSheet("background-color: purple;");
    ui->colorYellowButton->setStyleSheet("background-color: yellow;");
    ui->colorOrangeButton->setStyleSheet("background-color: orange;");
}

Ekran::~Ekran()
{
    delete ui;
}

void Ekran::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(0, 0, width(), height()/2, Qt::gray);
    p.fillRect(0, height()/2, width(), height()/2, Qt::gray);

    p.drawImage(0, 50, im);
}

//Metody do rysowania
void Ekran::drawPixel(int x, int y, int r, int g, int b)
{
    if(x <=0 || x >= im.width() || y <= 0 || y >= im.height())
        return;

    uchar *pix = im.scanLine(y);
    pix[4*x] = r;
    pix[4*x + 1] = g;
    pix[4*x + 2] = b;
    update();
}

void Ekran::drawLine(int x1, int y1, int x2, int y2)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    int k;
    float x = x1;
    float y = y1;

    if(abs(dx) > abs(dy))
        k = abs(dx);
    else
        k = abs(dy);

    if(k != 0)
    {
        float mx = dx / (float)k;
        float my = dy / (float)k;

        drawPixel(Round(x), Round(y), color1, color2, color3);
        for(int i = 0; i < k; i++)
        {
            x += mx;
            y += my;
            drawPixel(Round(x), Round(y), color1, color2, color3);
        }
    }
}

void Ekran::drawCircle(int x1, int y1, int x2, int y2)
{
    float R = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    float y;

    for(int x = 0; x <= (sqrt(2)/2)*R; x++)
    {
        y = sqrt(pow(R, 2) - pow(x, 2));
        drawPixel(x + x1, Round(y) + y1, color1, color2, color3);
        drawPixel(x + x1, -1*Round(y) + y1, color1, color2, color3);
        drawPixel(-x + x1, Round(y) + y1, color1, color2, color3);
        drawPixel(-x + x1, -1*Round(y) + y1, color1, color2, color3);
        drawPixel(Round(y) + x1, x + y1, color1, color2, color3);
        drawPixel(Round(y) + x1, -x + y1, color1, color2, color3);
        drawPixel(-1*Round(y) + x1, x + y1, color1, color2, color3);
        drawPixel(-1*Round(y) + x1, -x + y1, color1, color2, color3);
    }
}

void Ekran::drawElipse(int x0, int y0, int R1, int R2)
{
    float kat, x, y, x_last, y_last;

    for(int i = 0; i <= n; i++)
    {
        kat = (2*M_PI / n) * i;
        x = R1 * cos(kat);
        y = R2 * sin(kat);
        if(i == 0)
        {
            x_last = x;
            y_last = y;
        }
        drawLine(x + x0, y + y0, x_last + x0, y_last + y0);
        x_last = x;
        y_last = y;
    }
}

int Ekran::Round(int a)
{
    return (int)(a + 0.5);
}

void Ekran::mouseMoveEvent(QMouseEvent *e)
{    
    m2 = e->pos();
    int y1 = m1.y() - imageShift;
    int y2 = m2.y() - imageShift;
    if(buttonStatus != 0)
        im = tmp;

    if(m1.x() <= 0 || m1.x() >= im.width() || m1.y() <= imageShift || m1.y() >= im.height() + imageShift)
        return;
    else if(buttonStatus == 0)
    {
        drawPixel(m2.x(), y2, color1, color2, color3);
    }
    else if(buttonStatus == 1)
    {
        drawLine(m1.x(), y1, m2.x(), y2);
    }
    else if(buttonStatus == 2)
    {
        drawCircle(m1.x(), y1, m2.x(), y2);
    }
    else if(buttonStatus == 3)
    {
        float R1, R2, x0, y0;

        R1 = abs(m2.x() - m1.x()) / 2;
        R2 = abs(y2 - y1) / 2;

        x0 = (m1.x() + m2.x()) / 2;
        y0 = (y1 + y2) / 2;

        drawElipse(x0, y0, R1, R2);
    }
}

void Ekran::mousePressEvent(QMouseEvent *e)
{
    m1 = e->pos();
    int y = m1.y() - imageShift;
    outside_the_area = false;

    if(m1.x() > 0 && m1.x() < im.width() && m1.y() > imageShift && m1.y() < im.height() + imageShift)
        qDebug("x1: %d   y1: %d", m1.x(), m1.y());
    else
    {
        qDebug("Rysujesz poza obszarem");
        outside_the_area = true;
    }

    if(buttonStatus == 0 || buttonStatus == 1)
        drawPixel(m1.x(), y, color1, color2, color3);

    tmp = im;
}

void Ekran::mouseReleaseEvent(QMouseEvent *e)
{
    m2 = e->pos();

    if(m2.x() > 0 && m2.x() < im.width() && m2.y() > imageShift && m2.y() < im.height() + imageShift && outside_the_area == false)
        qDebug("x2: %d   y2: %d", m2.x(), m2.y());
    else if(outside_the_area == true)
        return;
    else
        qDebug("Rysujesz poza obszarem");
}

//Metody z UI
void Ekran::on_slider1_valueChanged(int value)
{
    n = value;

    QString qstr = "Wartość n = " + QString::number(n);
    ui->label1->setText(qstr);
}

void Ekran::on_cleanButton_clicked()
{
    im.fill(0);
    update();
}

void Ekran::on_resetSliderButton_clicked()
{
    ui->slider1->setValue(3);
    ui->label1->setText("Wartość n = 3");
}

void Ekran::on_quitButton_clicked()
{
    qApp->quit();
}

void Ekran::on_lineButton_clicked()
{
    buttonStatus = 1;
}

void Ekran::on_circleButton_clicked()
{
    buttonStatus = 2;
}

void Ekran::on_elipseButton_clicked()
{
    buttonStatus = 3;
}

void Ekran::on_randomColorButton_clicked()
{
    color1 = rand() % 255 + 1;
    color2 = rand() % 255 + 1;
    color3 = rand() % 255 + 1;
    qDebug("Wylosowane kolory to: red = %d green = %d blue = %d", color1, color2, color3);
}

void Ekran::on_drawButton_clicked()
{
    buttonStatus = 0;
}

void Ekran::on_colorRedButton_clicked()
{
    color1 = 0;
    color2 = 0;
    color3 = 255;
}

void Ekran::on_colorGreenButton_clicked()
{
    color1 = 0;
    color2 = 255;
    color3 = 0;
}

void Ekran::on_colorBlueButton_clicked()
{
    color1 = 255;
    color2 = 0;
    color3 = 0;
}

void Ekran::on_colorWhiteButton_clicked()
{
    color1 = 255;
    color2 = 255;
    color3 = 255;
}

void Ekran::on_colorBlackButton_clicked()
{
    color1 = 0;
    color2 = 0;
    color3 = 0;
}

void Ekran::on_colorPurpleButton_clicked()
{
    color1 = 128;
    color2 = 0;
    color3 = 128;
}

void Ekran::on_colorYellowButton_clicked()
{
    color1 = 0;
    color2 = 255;
    color3 = 255;
}

void Ekran::on_colorOrangeButton_clicked()
{
    color1 = 0;
    color2 = 140;
    color3 = 255;
}

