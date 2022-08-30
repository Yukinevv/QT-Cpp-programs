#include "ekran.h"
#include "ui_ekran.h"

#include <QMouseEvent>
#include <QPainter>
#include <cmath>

Ekran::Ekran(QWidget *parent)
            : QWidget(parent),
              ui(new Ui::Ekran)
{
    ui->setupUi(this);

    sourceImg = new QImage(":panda.jpg");
    blackImg = new QImage(sourceImg->width(), sourceImg->height(), QImage::Format_RGB32);
    sourceTriangle = new Triangle();
    blackImgTriangle = new Triangle();

    sourceMode = 0; // 0 - tryb wstawiania punktow | 1 - tryb przesuwania
    blackImgMode = 0;
    sourceTriangleVert = 0;
    blackImgTriangleVert = 0;
}

Ekran::~Ekran()
{
    delete ui;
}

void Ekran::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(0, 0, width(), height(), QColor(50, 50, 50));
    p.drawImage(0, 0, *sourceImg);
    p.drawImage(sourceImg->width(), 0, *blackImg);
    update();
}

void Ekran::appendVertices(QList<Point> &vertices, bool source)
{
    if (source == true)
    {
        vertices.append(sourceTriangle->a);
        vertices.append(sourceTriangle->b);
        vertices.append(sourceTriangle->c);
    }
    else
    {
        vertices.append(blackImgTriangle->a);
        vertices.append(blackImgTriangle->b);
        vertices.append(blackImgTriangle->c);
    }
}

void Ekran::drawPixel(Point p, bool source)
{
    if ((p.x < 0) || (p.y < 0) || (p.x > sourceImg->width() * 2) || (p.y > sourceImg->height()))
        return;

    int x = p.x, y = p.y;
    uchar *pix;

    if (source == true)
        pix = sourceImg->scanLine(y);
    else
        pix = blackImg->scanLine(y);

    pix[4*x] = 255;
    pix[4*x + 1] = 255;
    pix[4*x + 2] = 0;
}

void Ekran::drawPoint(Point p, bool source)
{
    if ((p.x < 0) || (p.y < 0) || (p.x >= sourceImg->width() * 2) || (p.y >= sourceImg->height()))
        return;

    for (int x = -5; x < 5; x++)
    {
        for (int y = -5; y < 5; y++)
        {
            drawPixel({p.x + x, p.y + y}, source);
        }
    }
}

void Ekran::drawLine(Point p1, Point p2, bool source)
{
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    int k;
    float x = p1.x;
    float y = p1.y;

    if (abs(dx) > abs(dy))
        k = abs(dx);
    else
        k = abs(dy);

    if (k != 0)
    {
        float mx = dx / (float)k;
        float my = dy / (float)k;

        drawPixel({round(x), round(y)}, source);
        for (int i = 0; i < k; i++)
        {
            x += mx;
            y += my;
            drawPixel({round(x), round(y)}, source);
        }
    }
}

int Ekran::round(int a)
{
    return (int)(a + 0.5);
}

void Ekran::drawTriangle(Point a, Point b, Point c, bool source)
{
    drawLine(a, b, source);
    drawLine(b, c, source);
    drawLine(c, a, source);
}

void Ekran::findClosestVert(Point p, bool source)
{
    int x = p.x, y = p.y, indexOfPointToMove;
    float minDistance = minD, distance;
    Point currentVertice;
    QList<Point> vertices;
    appendVertices(vertices, source);

    // szukanie najblizszego punktu - wierzcholka
    for (int i = 0; i < vertices.size(); i++)
    {
        currentVertice = vertices.at(i);
        distance = std::pow((x - currentVertice.x), 2) + std::pow((y - currentVertice.y), 2);

        if (distance < minDistance)
        {
            indexOfPointToMove = i;
            minDistance = distance;
        }
    }

    if (source == true)
        indexSource = indexOfPointToMove;
    else
        indexBlackImg = indexOfPointToMove;
}

void Ekran::barycentricCoordinates(Triangle triangle, Point p)
{
    int x = p.x, y = p.y;
    int x_a = triangle.a.x, y_a = triangle.a.y,
        x_b = triangle.b.x, y_b = triangle.b.y,
        x_c = triangle.c.x, y_c = triangle.c.y;
    float u, v, w;

    v = (float)(((x - x_a) * (y_c - y_a) - (y - y_a) * (x_c - x_a))) /
        (float)(((x_b - x_a) * (y_c - y_a) - (y_b - y_a) * (x_c - x_a)));

    w = (float)(((x_b - x_a) * (y - y_a) - (y_b - y_a) * (x - x_a))) /
        (float)(((x_b - x_a) * (y_c - y_a) - (y_b - y_a) * (x_c - x_a)));
    u = 1 - v - w;

    int x_t, y_t;
    uchar *pix, *sourcepix;

    // jesli punkt znajduje sie wewnatrz trojkata to go zamalowuje
    if ((u > 0) && (v > 0) && (w > 0) && (u < 1) && (v < 1) && (w < 1))
    {
        // obliczam wspolrzedne (x_t, y_t) punktu P_t na teksturze
        x_t = floor(u * sourceTriangle->a.x + v * sourceTriangle->b.x + w * sourceTriangle->c.x);
        y_t = floor(u * sourceTriangle->a.y + v * sourceTriangle->b.y + w * sourceTriangle->c.y);

        pix = blackImg->scanLine(y);
        sourcepix = sourceImg->scanLine(y_t);

        // koloruje punkt P wartoscia punktu P_t
        pix[4*x] = sourcepix[4*x_t];
        pix[4*x + 1] = sourcepix[4*x_t + 1];
        pix[4*x + 2] = sourcepix[4*x_t + 2];
    }
}

void Ekran::triangleTexturing()
{
    // dla kazdego punktu czarnego trojkata obliczam wspolrzedne barycentryczne
    for (int i = 0; i < blackImg->height(); i++)
    {
        for (int j = sourceImg->width(); j < sourceImg->width() + blackImg->width(); j++)
        {
            barycentricCoordinates(*blackImgTriangle, {j, i});
        }
    }
}

void Ekran::mousePressEvent(QMouseEvent *e)
{
    QPoint m = e->pos();
    Point p = Point(round(m.x()), round(m.y()));

    if ((p.x < 10) || (p.y < 10) || (p.x >= sourceImg->width() * 2 - 10) || (p.y >= sourceImg->height() - 10))
        return;

    if (p.x < sourceImg->width())
    {
        if (sourceMode == 0)
        {
            // dodaje wierzcholek na teksturze
            if (sourceTriangleVert == 0)
            {
                sourceTriangle->a = p;
                drawPoint(p, true);
            }
            else if (sourceTriangleVert == 1)
            {
                sourceTriangle->b = p;
                drawPoint(p, true);
            }
            else if (sourceTriangleVert == 2)
            {
                sourceTriangle->c = p;
                drawPoint(p, true);

                // rysujemy boki trojkata
                drawTriangle(sourceTriangle->a, sourceTriangle->b, sourceTriangle->c, true);

                if (blackImgTriangleVert > 2)
                {
                    triangleTexturing();
                }
            }
            else
            {
                findClosestVert(p, true);

                sourceMode = 1; // tryb przesuwania
            }
            sourceTriangleVert++;
        }
        else
        {
            // przesuwam wierzcholki na teksturze
            findClosestVert(p, true);
        }
    }
    else
    {
        if (blackImgMode == 0)
        {
            // dodaje wierzcholek na czarnym obrazku
            if (blackImgTriangleVert == 0)
            {
                blackImgTriangle->a = p;
                drawPoint(p, false);
            }
            else if (blackImgTriangleVert == 1)
            {
                blackImgTriangle->b = p;
                drawPoint(p, false);
            }
            else if (blackImgTriangleVert == 2)
            {
                blackImgTriangle->c = p;
                drawPoint(p, false);

                // rysuje boki trojkata
                drawTriangle(blackImgTriangle->a, blackImgTriangle->b, blackImgTriangle->c, false);

                if (sourceTriangleVert > 2)
                {
                    triangleTexturing();
                }
            }
            else
            {
                findClosestVert(p, false);

                blackImgMode = 1; // tryb przesuwania
            }
            blackImgTriangleVert++;
        }
        else
        {
            // przesuwam wierzcholek na czarnym obrazku
            findClosestVert(p, false);
        }
    }
}

void Ekran::mouseMoveEvent(QMouseEvent *e)
{
    QPoint m = e->pos();
    Point np = Point(round(m.x()), round(m.y()));

    if ((np.x < 10) || (np.y < 10) || (np.x >= sourceImg->width() * 2 - 10) || (np.y >= sourceImg->height() - 10))
        return;

    if (sourceMode != 0)
    {
        if (np.x < sourceImg->width())
        {
            // przesuwam po teksturze
            if (indexSource == 0)
            {
                sourceTriangle->a = np;
            }
            else if (indexSource == 1)
            {
                sourceTriangle->b = np;
            }
            else
            {
                sourceTriangle->c = np;
            }

            clearSource();

            drawPoint(sourceTriangle->a, true);
            drawPoint(sourceTriangle->b, true);
            drawPoint(sourceTriangle->c, true);

            drawTriangle(sourceTriangle->a, sourceTriangle->b, sourceTriangle->c, true);
        }
    }

    if (blackImgMode != 0)
    {
        if (np.x > sourceImg->width())
        {
            // przesuwam po czarnym obrazku
            if (indexBlackImg == 0)
            {
                blackImgTriangle->a = np;
            }
            else if (indexBlackImg == 1)
            {
                blackImgTriangle->b = np;
            }
            else
            {
                blackImgTriangle->c = np;
            }

            clearBlackImg();

            drawPoint(blackImgTriangle->a, false);
            drawPoint(blackImgTriangle->b, false);
            drawPoint(blackImgTriangle->c, false);

            drawTriangle(blackImgTriangle->a, blackImgTriangle->b, blackImgTriangle->c, false);
        }
    }

    if ((sourceTriangleVert > 2) && (blackImgTriangleVert > 2))
    {
        triangleTexturing();
    }
}

void Ekran::clearSource()
{
    sourceImg = new QImage(":panda.jpg");
    update();
}

void Ekran::clearBlackImg()
{
    blackImg = new QImage(sourceImg->width(), sourceImg->height(), QImage::Format_RGB32);
    update();
}
