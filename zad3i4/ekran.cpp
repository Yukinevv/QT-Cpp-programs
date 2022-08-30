#include "ekran.h"
#include "ui_ekran.h"

#include <QMouseEvent>
#include <QPainter>

Ekran::Ekran(QWidget *parent)
    : QWidget{parent},
      im(1250, 500, QImage::Format_RGB32),
      tmp(1250, 500, QImage::Format_RGB32),
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

void Ekran::drawBezierCurves(std::vector<Punkt> pkt)
{
    int n = 5; //ilosc segmentow krzywej
    int N = (((int)pkt.size() - 1) / 3);
    float t;
    Punkt P, Q;
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < n; j++)
        {
            t = j / (float)n;
            P.x = pow((1-t), 3)*pkt[3*i].x + 3*pow((1-t), 2)*t*pkt[3*i+1].x + 3*(1-t)*pow(t, 2)*pkt[3*i+2].x + pow(t, 3)*pkt[3*i+3].x;
            P.y = pow((1-t), 3)*pkt[3*i].y + 3*pow((1-t), 2)*t*pkt[3*i+1].y + 3*(1-t)*pow(t, 2)*pkt[3*i+2].y + pow(t, 3)*pkt[3*i+3].y;
            t = t + ((float)1 / float(n));
            Q.x = pow((1-t), 3)*pkt[3*i].x + 3*pow((1-t), 2)*t*pkt[3*i+1].x + 3*(1-t)*pow(t, 2)*pkt[3*i+2].x + pow(t, 3)*pkt[3*i+3].x;
            Q.y = pow((1-t), 3)*pkt[3*i].y + 3*pow((1-t), 2)*t*pkt[3*i+1].y + 3*(1-t)*pow(t, 2)*pkt[3*i+2].y + pow(t, 3)*pkt[3*i+3].y;
            drawLine(P.x, P.y, Q.x, Q.y);
        }
    }
}

void Ekran::drawBSplineCurves(std::vector<Punkt> pkt)
{
    int n = 5;
    float t;
    Punkt P, Q;
    for(int i = 3; i < (int)pkt.size(); i++)
    {
        for(int j = 0; j < n; j++)
        {
            t = j / (float)n;
            P.x = ( -pow(t, 3) + 3*pow(t, 2) - 3*t + 1 ) / 6 * pkt[i-3].x + ( 3*pow(t, 3) - 6*pow(t, 2) + 4 ) / 6 * pkt[i-2].x
                    + ( -3*pow(t, 3) + 3*pow(t, 2) + 3*t + 1 ) / 6 * pkt[i-1].x + pow(t, 3) / 6 * pkt[i].x;
            P.y = ( -pow(t, 3) + 3*pow(t, 2) - 3*t + 1 ) / 6 * pkt[i-3].y + ( 3*pow(t, 3) - 6*pow(t, 2) + 4 ) / 6 * pkt[i-2].y
                    + ( -3*pow(t, 3) + 3*pow(t, 2) + 3*t + 1 ) / 6 * pkt[i-1].y + pow(t, 3) / 6 * pkt[i].y;
            t = t + ((float)1 / float(n));
            Q.x = ( -pow(t, 3) + 3*pow(t, 2) - 3*t + 1 ) / 6 * pkt[i-3].x + ( 3*pow(t, 3) - 6*pow(t, 2) + 4 ) / 6 * pkt[i-2].x
                    + ( -3*pow(t, 3) + 3*pow(t, 2) + 3*t + 1 ) / 6 * pkt[i-1].x + pow(t, 3) / 6 * pkt[i].x;
            Q.y = ( -pow(t, 3) + 3*pow(t, 2) - 3*t + 1 ) / 6 * pkt[i-3].y + ( 3*pow(t, 3) - 6*pow(t, 2) + 4 ) / 6 * pkt[i-2].y
                    + ( -3*pow(t, 3) + 3*pow(t, 2) + 3*t + 1 ) / 6 * pkt[i-1].y + pow(t, 3) / 6 * pkt[i].y;
            drawLine(P.x, P.y, Q.x, Q.y);
        }
    }
}

int Ekran::findIndexOfClosestPoint(Punkt P, std::vector<Punkt> pkt)
{
    int index;
    float distance[(int)pkt.size() + 1];

    for(int i = 0; i < (int)pkt.size(); i++)
    {
        distance[i] = sqrt(pow(P.x - pkt[i].x, 2) + pow(P.y - pkt[i].y, 2));
        qDebug("Distance[%d]: %f", i, distance[i]);
    }
    float minimum = distance[0];
    index = 0;
    for(int i = 1; i < (int)pkt.size(); i++)
    {
        if(minimum > distance[i])
        {
            minimum = distance[i];
            index = i;
        }
    }
    return index;
}

void Ekran::drawPoints(std::vector<Punkt> points, int r, int g, int b)
{
    for(int i = 0; i < (int)points.size(); i++)
        drawPixel(points[i].x, points[i].y, r, g, b);
}

//Events
void Ekran::mousePressEvent(QMouseEvent *e)
{
    m1 = e->pos();
    outside_the_area = false;

    Punkt P;
    if(buttonStatus == 4 || buttonStatus == 5) //zad 3 - Bezier Curves / BSpline Curves
    {
        if(m1.x() <= 0 || m1.x() >= im.width() || m1.y() <= 0 || m1.y() >= im.height())
            qDebug("Rysujesz poza obszarem");
        else
        {
            P.x = m1.x();
            P.y = m1.y();
        }
    }
    else //Pozostale zadania
    {
        if(m1.x() > 0 && m1.x() < im.width() && m1.y() > 0 && m1.y() < im.height())
            qDebug("x1: %d   y1: %d", m1.x(), m1.y());
        else
        {
            qDebug("Rysujesz poza obszarem");
            outside_the_area = true;
        }
    }

    if(buttonStatus == 0 || buttonStatus == 1)
        drawPixel(m1.x(), m1.y(), color1, color2, color3);

    //zad 3 - Bezier Curves / BSpline Curves
    if((buttonStatus == 4 || buttonStatus == 5) && (m1.x() > 0 && m1.x() < im.width() && m1.y() > 0 && m1.y() < im.height())
            && e->button() == Qt::LeftButton)
    {
        if(buttonStatus == 4)
        {
            drawPixel(P.x, P.y, 0, 255, 0);
            pointsBezier.push_back(P);

            pktBezier.push_back(P);
            drawBezierCurves(pktBezier);
        }
        else if(buttonStatus == 5)
        {
            drawPixel(P.x, P.y, 0, 0, 255);
            pointsBSpline.push_back(P);

            pktBSpline.push_back(P);
            drawBSplineCurves(pktBSpline);
        }
    }
    else if(e->button() == Qt::MiddleButton && (buttonStatus == 4 || buttonStatus == 5))
    {
        if((int)pktBezier.size() > 0 && buttonStatus == 4)
        {
            indexClosestPoint = findIndexOfClosestPoint(P, pktBezier);
            if((int)pointsBezier.size() > 0)
                pointsBezier.erase(pointsBezier.begin() + indexClosestPoint);
            qDebug("Usuwasz punkt nr %d", indexClosestPoint);

            pktBezier.erase(pktBezier.begin() + indexClosestPoint);
            im.fill(0);
            drawPoints(pointsBezier, 0, 255, 0);
            drawBezierCurves(pktBezier);
            drawBSplineCurves(pktBSpline);
            update();
        }
        else if((int)pktBSpline.size() > 0 && buttonStatus == 5)
        {
            indexClosestPoint = findIndexOfClosestPoint(P, pktBSpline);
            if((int)pointsBSpline.size() > 0)
                pointsBSpline.erase(pointsBSpline.begin() + indexClosestPoint);
            qDebug("Usuwasz punkt nr %d", indexClosestPoint);

            pktBSpline.erase(pktBSpline.begin() + indexClosestPoint);
            im.fill(0);
            drawPoints(pointsBSpline, 0, 0, 255);
            drawBezierCurves(pktBezier);
            drawBSplineCurves(pktBSpline);
            update();
        }
        else
            qDebug("Brak punktow do usuwania!");
    }
    else if(e->button() == Qt::RightButton && (buttonStatus == 4 || buttonStatus == 5))
    {
        if((int)pktBezier.size() > 0 && buttonStatus == 4)
        {
            moveBezier = true;
            indexClosestPoint = findIndexOfClosestPoint(P, pktBezier);
            std::swap(pointsBezier[indexClosestPoint].x, P.x);
            std::swap(pointsBezier[indexClosestPoint].y, P.y);
            qDebug("Przesuwasz punkt nr %d", indexClosestPoint);

            std::swap(pktBezier[indexClosestPoint].x, P.x);
            std::swap(pktBezier[indexClosestPoint].y, P.y);

            im.fill(0);
            drawPoints(pointsBezier, 0, 255, 0);
            drawBezierCurves(pktBezier);
            drawBSplineCurves(pktBSpline);
        }
        else if((int)pktBSpline.size() > 0 && buttonStatus == 5)
        {
            moveBezier = true;
            indexClosestPoint = findIndexOfClosestPoint(P, pktBSpline);
            std::swap(pointsBSpline[indexClosestPoint].x, P.x);
            std::swap(pointsBSpline[indexClosestPoint].y, P.y);
            qDebug("Przesuwasz punkt nr %d", indexClosestPoint);

            std::swap(pktBSpline[indexClosestPoint].x, P.x);
            std::swap(pktBSpline[indexClosestPoint].y, P.y);

            im.fill(0);
            drawPoints(pointsBSpline, 0, 0, 255);
            drawBezierCurves(pktBezier);
            drawBSplineCurves(pktBSpline);
        }
        else
            qDebug("Brak punktow do przesuwania!");
    }

    tmp = im;
}

void Ekran::mouseMoveEvent(QMouseEvent *e)
{
    m2 = e->pos();
    if(buttonStatus != 0)
        im = tmp;

    if(m1.x() <= 0 || m1.x() >= im.width() || m1.y() <= 0 || m1.y() >= im.height())
        return;
    else if(buttonStatus == 0)
    {
        drawPixel(m2.x(), m2.y(), color1, color2, color3);
    }
    else if(buttonStatus == 1)
    {
        drawLine(m1.x(), m1.y(), m2.x(), m2.y());
    }
    else if(buttonStatus == 2)
    {
        drawCircle(m1.x(), m1.y(), m2.x(), m2.y());
    }
    else if(buttonStatus == 3)
    {
        float R1, R2, x0, y0;

        R1 = abs(m2.x() - m1.x()) / 2;
        R2 = abs(m2.y() - m1.y()) / 2;

        x0 = (m1.x() + m2.x()) / 2;
        y0 = (m1.y() + m2.y()) / 2;

        drawElipse(x0, y0, R1, R2);
    }
    else if((buttonStatus == 4 || buttonStatus == 5) && moveBezier == true) //tylko gdy zostal wcisniety prawy przycisk myszy (w MousePressEvent)
    {                                                                       //oraz if(pktBezier.size() > 0) else if(pktBSpline.size() > 0)
        Punkt Q;
        Q.x = m2.x();
        Q.y = m2.y();

        if(buttonStatus == 4)
        {
            std::swap(pointsBezier[indexClosestPoint].x, Q.x);
            std::swap(pointsBezier[indexClosestPoint].y, Q.y);

            std::swap(pktBezier[indexClosestPoint].x, Q.x);
            std::swap(pktBezier[indexClosestPoint].y, Q.y);
        }
        else if(buttonStatus == 5)
        {
            std::swap(pointsBSpline[indexClosestPoint].x, Q.x);
            std::swap(pointsBSpline[indexClosestPoint].y, Q.y);

            std::swap(pktBSpline[indexClosestPoint].x, Q.x);
            std::swap(pktBSpline[indexClosestPoint].y, Q.y);
        }
        im.fill(0);

        if(buttonStatus == 4)
            drawPoints(pointsBezier, 0, 255, 0);
        else if(buttonStatus == 5)
            drawPoints(pointsBSpline, 0, 0, 255);

        drawBezierCurves(pktBezier);
        drawBSplineCurves(pktBSpline);
    }
}

void Ekran::mouseReleaseEvent(QMouseEvent *e)
{
    m2 = e->pos();

    if(buttonStatus == 4 || buttonStatus == 5) //zad 3 - Bezier Curves / BSpline Curves
        moveBezier = false;
    else //Pozostale zadania
    {
        if(m2.x() > 0 && m2.x() < im.width() && m2.y() > 0 && m2.y() < im.height() && outside_the_area == false)
            qDebug("x2: %d   y2: %d", m2.x(), m2.y());
        else if(outside_the_area == true)
            return;
        else
            qDebug("Rysujesz poza obszarem");
    }
}

void Ekran::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(0, 0, width(), height()/2, Qt::gray);
    p.fillRect(0, height()/2, width(), height()/2, Qt::gray);

    p.drawImage(0, 0, im);
}

//Metody z UI
void Ekran::on_cleanButton_clicked()
{
    im.fill(0);
    update();

    //zad 3 - Bezier Curves / BSpline Curves
    if(buttonStatus == 4 || buttonStatus == 5)
    {
        pktBezier.clear();
        pktBSpline.clear();
        pointsBezier.clear();
        pointsBSpline.clear();
    }
}

void Ekran::on_slider1_valueChanged(int value)
{
    n = value;

    QString qstr = "Wartość n = " + QString::number(n);
    ui->label1->setText(qstr);
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

void Ekran::on_drawButton_clicked()
{
    buttonStatus = 0;
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

void Ekran::on_bezierCurveButton_clicked()
{
    buttonStatus = 4;
}

void Ekran::on_bsplineCurveButton_clicked()
{
    buttonStatus = 5;
}

//Metody do rysowania z poprzednich zadan
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

//Kolory (UI)
void Ekran::on_randomColorButton_clicked()
{
    color1 = rand() % 255 + 1;
    color2 = rand() % 255 + 1;
    color3 = rand() % 255 + 1;
    qDebug("Wylosowane kolory to: red = %d green = %d blue = %d", color1, color2, color3);
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

