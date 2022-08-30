#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>

class Ekran : public QWidget
{
    Q_OBJECT
public:
    QImage im, tmp;
    QPoint m1, m2;
    bool outside_the_area;
    explicit Ekran(QWidget *parent = nullptr);
    void drawPixel(int x, int y, int r, int g, int b);
    void drawLine(int x1, int y1, int x2, int y2);
    int Round(int a);
protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
signals:

};

#endif // EKRAN_H
