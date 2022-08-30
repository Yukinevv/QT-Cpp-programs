#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>

namespace Ui { class Ekran; }

class Ekran : public QWidget
{
    Q_OBJECT
public:
    explicit Ekran(QWidget *parent = nullptr);
    ~Ekran();

    QImage im, tmp;
    QPoint m1, m2;
    bool outside_the_area = false;

    int kR = 0, kG = 0, kB = 0;
    int kH = 0, kS = 0, kV = 0;
    float rysR = 0, rysG = 0, rysB = 0;

    void drawPixel(int x, int y, int r, int g, int b);
    void drawLine(int x1, int y1, int x2, int y2);
    int Round(int a);
    void drawSquareRGB(char whichColor);
    void drawSquareHSV(char whichSlider);
    void HSVToRGB(float H, float S, float V, bool local);
    void RGBToHSV(float R, float G, float B);
    float maxValue(float x, float y, float z);
    float minValue(float x, float y, float z);
    void moveSliders();
protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
signals:

private slots:
    void on_quitButton_clicked();

    void on_sliderRed_sliderMoved(int position);

    void on_sliderGreen_sliderMoved(int position);

    void on_sliderBlue_sliderMoved(int position);

    void on_sliderH_sliderMoved(int position);

    void on_sliderS_sliderMoved(int position);

    void on_sliderV_sliderMoved(int position);

private:
    Ui::Ekran *ui;
};

#endif // EKRAN_H
