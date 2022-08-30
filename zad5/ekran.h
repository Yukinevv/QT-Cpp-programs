#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>
#include <QHBoxLayout>
#include <stack>

namespace Ui { class Ekran; }

class Ekran : public QWidget
{
    Q_OBJECT
public:
    explicit Ekran(QWidget *parent = nullptr);
    ~Ekran();

    QImage im, tmp;
    QPoint m1, m2;

    std::stack<QPoint> pointStack;

    int buttonStatus = 1;
    bool outside_the_area;
    int color1 = 255, color2 = 255, color3 = 255;

    void drawPixel(int x, int y, int r, int g, int b);
    void drawLine(int x1, int y1, int x2, int y2);
    void drawCircle(int x1, int y1, int x2, int y2);
    int Round(int a);
    void floodFill(int x, int y, QRgb color, int newColor1, int newColor2, int newColor3);
    QRgb getColor(int x, int y);
protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
signals:

private slots:
    void on_cleanButton_clicked();

    void on_quitButton_clicked();

    void on_randomColorButton_clicked();

    void on_lineButton_clicked();

    void on_circleButton_clicked();

    void on_floodFillButton_clicked();

private:
    Ui::Ekran *ui;
};

#endif // EKRAN_H
