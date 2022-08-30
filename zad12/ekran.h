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

class Ekran : public QWidget
{
    Q_OBJECT
public:
    explicit Ekran(QWidget *parent = nullptr);

    QImage *img1, *img2;
    int pointsImg1, pointsImg2;
    int currImg;
    Point vertImg1[3], vertImg2[3];
    int currIndex;
    Triangle *triangle1, *triangle2;

    void drawPixel(Point p);
    void drawPoint(Point p);
    void drawLine(Point p1, Point p2);
    int round(int a);
    void drawTriangle();
    void findClosestVert(Point p);
    void clearImg();
    void clearAndDrawImg(Point p);
    void changeImage(int value);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

signals:

private slots:

private:
    int minD = 1000000;
};

#endif // EKRAN_H
