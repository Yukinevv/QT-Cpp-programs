#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>
#include <QHBoxLayout>
#include <punkt.h>

namespace Ui { class Ekran; }

class Ekran : public QWidget
{
    Q_OBJECT
public:
    explicit Ekran(QWidget *parent = nullptr);
    ~Ekran();

    QImage im, tmp;
    QPoint m1, m2;

    QFont font1;
    QPalette palette1;

    std::vector<Punkt> pktBezier;
    std::vector<Punkt> pktBSpline;
    std::vector<Punkt> pointsBezier;
    std::vector<Punkt> pointsBSpline;
    bool moveBezier = false;
    int indexClosestPoint;

    int buttonStatus = 1;
    bool outside_the_area;
    int n = 3;
    int color1 = 255;
    int color2 = 255;
    int color3 = 255;

    void drawPixel(int x, int y, int r, int g, int b);
    void drawLine(int x1, int y1, int x2, int y2);
    void drawCircle(int x1, int y1, int x2, int y2);
    void drawElipse(int x1, int y1, int R1, int R2);
    void drawBezierCurves(std::vector<Punkt> pkt);
    void drawBSplineCurves(std::vector<Punkt> pkt);
    int Round(int a);
    int findIndexOfClosestPoint(Punkt P, std::vector<Punkt> pkt);
    void drawPoints(std::vector<Punkt> points, int r, int g, int b);
protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
signals:

private slots:
    void on_slider1_valueChanged(int value);

    void on_cleanButton_clicked();

    void on_resetSliderButton_clicked();

    void on_quitButton_clicked();

    void on_lineButton_clicked();

    void on_circleButton_clicked();

    void on_elipseButton_clicked();

    void on_randomColorButton_clicked();

    void on_drawButton_clicked();

    void on_colorRedButton_clicked();

    void on_colorGreenButton_clicked();

    void on_colorBlueButton_clicked();

    void on_colorWhiteButton_clicked();

    void on_colorBlackButton_clicked();

    void on_colorPurpleButton_clicked();

    void on_colorYellowButton_clicked();

    void on_colorOrangeButton_clicked();

    void on_bezierCurveButton_clicked();

    void on_bsplineCurveButton_clicked();

private:
    Ui::Ekran *ui;
};

#endif // EKRAN_H
