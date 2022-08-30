#include "ekran.h"

#include <QMouseEvent>
#include <QPainter>

Ekran::Ekran(QWidget *parent)
    : QWidget{parent},
      im(500, 400, QImage::Format_RGB32),
      tmp(500, 400, QImage::Format_RGB32)
{
    im.fill(0);
}

void Ekran::drawPixel(int x, int y, int r, int g, int b)
{
    if(x <= 0 || x >= 500 || y <= 0 || y >= 400)
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

        drawPixel(Round(x), Round(y), 255, 255, 255);
        for(int i = 0; i < k; i++)
        {
            x += mx;
            y += my;
            drawPixel(Round(x), Round(y), 255, 255, 255);
        }
    }
}

int Ekran::Round(int a)
{
    return (int)(a + 0.5);
}

void Ekran::mouseMoveEvent(QMouseEvent *e)
{
    m2 = e->pos();

    im = tmp;

    if(m1.x() <= 0 || m1.x() >= 500 || m1.y() <= 0 || m1.y() >= 400)
        return;
    else
        drawLine(m1.x(), m1.y(), m2.x(), m2.y());
}

void Ekran::mousePressEvent(QMouseEvent *e)
{
    m1 = e->pos();
    outside_the_area = false;

    if(m1.x() > 0 && m1.x() < 500 && m1.y() > 0 && m1.y() < 400)
        qDebug("x1: %d   y1: %d", m1.x(), m1.y());
    else
    {
        qDebug("Rysujesz poza obszarem");
        outside_the_area = true;
    }

    drawPixel(m1.x(), m1.y(), 255, 255, 255);

    tmp = im;
}

void Ekran::mouseReleaseEvent(QMouseEvent *e)
{
    m2 = e->pos();

    if(m2.x() > 0 && m2.x() < 500 && m2.y() > 0 && m2.y() < 400 && outside_the_area == false)
        qDebug("x2: %d   y2: %d", m2.x(), m2.y());
    else if(outside_the_area == true)
        return;
    else
        qDebug("Rysujesz poza obszarem");
}

void Ekran::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(0, 0, width(), height(), Qt::green);

    p.drawImage(0, 0, im);
}
