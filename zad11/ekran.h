#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>

class Point
{
public:
    int x, y;

    Point()
    {
    }
    Point(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
};

class Triangle
{
public:
    Point a, b, c;

    Triangle()
    {
    }
    Triangle(Point a, Point b, Point c)
    {
        this->a = a;
        this->b = b;
        this->c = c;
    }
};

namespace Ui { class Ekran; }

class Ekran : public QWidget
{
    Q_OBJECT
public:
    explicit Ekran(QWidget *parent = nullptr);
    ~Ekran();

    QImage *sourceImg, *blackImg;
    Triangle *sourceTriangle, *blackImgTriangle;
    Point pointToMove;
    int sourceMode, blackImgMode;
    int sourceTriangleVert, blackImgTriangleVert;
    int indexSource, indexBlackImg;

    void drawPixel(Point p, bool source);
    void drawPoint(Point p, bool source);
    void drawLine(Point p1, Point p2, bool source);
    int round(int a);
    void drawTriangle(Point a, Point b, Point c, bool source);
    void appendVertices(QList<Point> &vertices, bool source);
    void findClosestVert(Point p, bool source);
    void barycentricCoordinates(Triangle triangle, Point p);
    void triangleTexturing();
    void clearSource();
    void clearBlackImg();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

signals:

private slots:

private:
    Ui::Ekran *ui;
    int minD = 1000000;
};

#endif // EKRAN_H
