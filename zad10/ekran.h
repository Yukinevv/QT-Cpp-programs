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

    uchar *pix, *pix2;
    QImage *img, *tmpImg, *pikachu;
    int height, width, movement, imageSize, scaleBoth;
    double moveMatrix[3][3], rotateMatrix[3][3], scaleMatrix[3][3], shearingMatrix[3][3], resultMatrix[3][3];

    void setImage();
    void drawPixel(uchar *pix, int x, int y, int color);
    void matrixMultiplication(double tab1[3][3], double tab2[3][3]);
    void matrixInversion(double M[3][3]);
    void clearImage(uchar *pix, bool flag);
    void matrixReset(double matrix[3][3]);
    void reset();
    void drawImage();

protected:
    void paintEvent(QPaintEvent *);
signals:

private slots:
    void on_moveXSlider_valueChanged(int value);

    void on_moveYSlider_valueChanged(int value);

    void on_spinSlider_valueChanged(int value);

    void on_scaleXSlider_valueChanged(int value);

    void on_scaleYSlider_valueChanged(int value);

    void on_shearingXSlider_valueChanged(int value);

    void on_shearingYSlider_valueChanged(int value);

    void on_checkBox_stateChanged(int arg1);

    void on_resetButton_clicked();

    void on_loadButton_clicked();

    void on_exitButton_clicked();

private:
    Ui::Ekran *ui;
};

#endif // EKRAN_H
