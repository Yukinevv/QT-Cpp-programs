#include "ekran.h"

#include <QImage>
#include <QPainter>
#include <QMouseEvent>

Ekran::Ekran(QWidget *parent)
            : QWidget(parent)
{
    img1 = new QImage(":zdj1.jpg");
    img2 = new QImage(":zdj2.jpg");

    currImg = 0;
    pointsImg1 = 0;
    pointsImg2 = 0;

    this->resize(500, 500);
}

void Ekran::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    if (currImg == 0)
    {
        p.drawImage(0, 0, *img1);
    }
    else
    {
        p.drawImage(0, 0, *img2);
    }
    update();
}

void Ekran::clearImg()
{
    if (currImg == 0)
    {
        img1 = new QImage(":zdj1.jpg");
    }
    else
    {
        img2 = new QImage(":zdj2.jpg");
    }
    update();
}

void Ekran::drawPixel(Point p)
{
    int x = p.x;
    int y = p.y;

    if ((x < 0) || (y < 0) || (x > 500) || (y > 500))
        return;

    uchar *ptr;
    if (currImg == 0)
        ptr = img1->scanLine(y);
    else
        ptr = img2->scanLine(y);

    ptr[4*x] = 255;
    ptr[4*x + 1] = 255;
    ptr[4*x + 2] = 0;
}

void Ekran::drawPoint(Point p)
{
    if ((p.x < 0) || (p.y < 0) || (p.x >= 500) || (p.y >= 500))
        return;

    for (int x = -5; x < 5; x++)
    {
        for (int y = -5; y < 5; y++)
        {
            drawPixel({p.x + x, p.y + y});
        }
    }
}

void Ekran::drawLine(Point p1, Point p2)
{
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    int k;
    float x = p1.x;
    float y = p1.y;

    if(abs(dx) > abs(dy))
        k = abs(dx);
    else
        k = abs(dy);

    if(k != 0)
    {
        float mx = dx / (float)k;
        float my = dy / (float)k;

        drawPixel({round(x), round(y)});
        for(int i = 0; i < k; i++)
        {
            x += mx;
            y += my;
            drawPixel({round(x), round(y)});
        }
    }
}

int Ekran::round(int a)
{
    return (int)(a + 0.5);
}

void Ekran::drawTriangle()
{
    if (currImg == 0)
    {
        drawLine(vertImg1[0], vertImg1[1]);
        drawLine(vertImg1[1], vertImg1[2]);
        drawLine(vertImg1[2], vertImg1[0]);
    }
    else
    {
        drawLine(vertImg2[0], vertImg2[1]);
        drawLine(vertImg2[1], vertImg2[2]);
        drawLine(vertImg2[2], vertImg2[0]);
    }
}

void Ekran::findClosestVert(Point p)
{
    double minDistance = minD, distance;
    if (currImg == 0)
    {
        for (int i = 0; i < 3; i++)
        {
            distance = std::pow((p.x - vertImg1[i].x), 2) + std::pow((p.y - vertImg1[i].y), 2);
            if (distance < minDistance)
            {
                currIndex = i;
                minDistance = distance;
            }
        }
    }
    else
    {
        for (int i = 0; i < 3; i++)
        {
            distance = std::pow((p.x - vertImg2[i].x), 2) + std::pow((p.y - vertImg2[i].y), 2);
            if (distance < minDistance)
            {
                currIndex = i;
                minDistance = distance;
            }
        }
    }
}

void Ekran::mousePressEvent(QMouseEvent *e)
{
    QPointF m = e->pos();
    Point p(round(m.x()), round(m.y()));

    if (currImg == 0)
    {
        if (pointsImg1 < 3)
        {
            drawPoint(p);
            vertImg1[pointsImg1] = p;
            pointsImg1++;
        }
        else
        {
            findClosestVert(p);
        }

        if (pointsImg1 == 3)
            drawTriangle();
    }
    else
    {
        if (pointsImg2 < 3)
        {
            drawPoint(p);
            vertImg2[pointsImg2] = p;
            pointsImg2++;
        }
        else
        {
            findClosestVert(p);
        }

        if (pointsImg2 == 3)
            drawTriangle();
    }
}

void Ekran::mouseMoveEvent(QMouseEvent *e)
{
    QPoint m = e->pos();
    Point p(round(m.x()), round(m.y()));

    clearAndDrawImg(p);
}

void Ekran::clearAndDrawImg(Point p)
{
    if (currImg == 0)
    {
        if (pointsImg1 >= 3)
        {
            clearImg();
            if (p.x > -1)
            {
                vertImg1[currIndex] = p;
            }
            drawPoint(vertImg1[0]);
            drawPoint(vertImg1[1]);
            drawPoint(vertImg1[2]);
            drawTriangle();
        }
    }
    else
    {
        if (pointsImg2 >= 3)
        {
            clearImg();
            if (p.x > -1)
            {
                vertImg2[currIndex] = p;
            }
            drawPoint(vertImg2[0]);
            drawPoint(vertImg2[1]);
            drawPoint(vertImg2[2]);
            drawTriangle();
        }
    }
}

void Ekran::changeImage(int value)
{
    currImg = value;
    update();
}
