#include "ekran.h"
#include "ui_ekran.h"

#include <QPainter>
#include <QFileDialog>

Ekran::Ekran(QWidget *parent)
            : QWidget(parent),
              ui(new Ui::Ekran)
{
    ui->setupUi(this);

    width = ui->frame->width();
    height = ui->frame->height();
    img = new QImage(width, height, QImage::Format_RGB32);
    clearImage(pix, false);
    pikachu = new QImage(":/pikachu.jpg");
    imageSize = pikachu->width();
    setImage();
}

Ekran::~Ekran()
{
    delete ui;
}

void Ekran::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(0, 0, *img);
}

void Ekran::clearImage(uchar *pix, bool flag)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (flag == false)
                drawPixel(pix, j, i, 0);
            else
                drawPixel(pix, j, i, 255);
        }
    }
    update();
}

void Ekran::matrixReset(double matrix[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            matrix[i][j] = 0;
        }
        matrix[i][i] = 1;
    }
}

void Ekran::reset()
{
    ui->moveXSlider->setValue(0);
    ui->moveYSlider->setValue(0);
    ui->spinSlider->setValue(0);
    ui->scaleXSlider->setValue(100);
    ui->scaleYSlider->setValue(100);
    ui->shearingXSlider->setValue(0);
    ui->shearingYSlider->setValue(0);
    matrixReset(moveMatrix);
    matrixReset(rotateMatrix);
    matrixReset(scaleMatrix);
    matrixReset(shearingMatrix);
    matrixReset(resultMatrix);
}

void Ekran::setImage()
{
    reset();
    moveMatrix[0][2] = 0;
    moveMatrix[1][2] = 0;
    drawImage();
}

void Ekran::drawPixel(uchar *pix, int x, int y, int c)
{
    if (width - x > 0 && width + x > width && height - y > 0 && height + y > height)
    {
        pix = img->scanLine(y);
        pix[4*x] = c;
        pix[4*x + 1] = c;
        pix[4*x + 2] = c;
    }
}

void Ekran::matrixMultiplication(double arr1[3][3], double arr2[3][3])
{
    double tmp;
    double newArr[3][3];

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            tmp = 0;
            for (int k = 0; k < 3; k++)
            {
                tmp += arr1[k][j] * arr2[i][k];
            }
            newArr[i][j] = tmp;
        }
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            arr2[i][j] = newArr[i][j];
        }
    }
}

void Ekran::matrixInversion(double M[3][3])
{
    // wyznacznik macierzy wymiaru 3x3
    double det = M[0][0] * M[1][1] * M[2][2] + M[1][0] * M[2][1] * M[0][2] + M[2][0] * M[0][1] * M[1][2] -
            (M[0][0] * M[2][1] * M[1][2]) - (M[2][1] * M[1][1] * M[0][2]) - (M[1][0] * M[0][1] * M[2][2]);
    if (det != 0)
    {
        det = 1 / det;
        double T[3][3];

        // macierz dopelnien
        T[0][0] = det * (M[1][1] * M[2][2] - (M[1][2] * M[2][1]));
        T[0][1] = det * (M[0][2] * M[2][1] - (M[0][1] * M[2][2]));
        T[0][2] = det * (M[0][1] * M[1][2] - (M[0][2] * M[1][1]));
        T[1][0] = det * (M[1][2] * M[2][0] - (M[1][0] * M[2][2]));
        T[1][1] = det * (M[0][0] * M[2][2] - (M[0][2] * M[2][0]));
        T[1][2] = det * (M[0][2] * M[1][0] - (M[0][0] * M[1][2]));
        T[2][0] = det * (M[1][0] * M[2][1] - (M[1][1] * M[2][0]));
        T[2][1] = det * (M[0][1] * M[2][0] - (M[0][0] * M[2][1]));
        T[2][2] = det * (M[0][0] * M[1][1] - (M[0][1] * M[1][0]));

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                M[i][j] = T[i][j];
            }
        }
    }
}

void Ekran::drawImage()
{
    uchar *pix, *pix2;
    pix = img->bits();

    clearImage(pix, true);
    matrixReset(resultMatrix);

    matrixInversion(rotateMatrix);
    matrixInversion(scaleMatrix);
    matrixInversion(shearingMatrix);

    matrixMultiplication(moveMatrix, resultMatrix);
    matrixMultiplication(rotateMatrix, resultMatrix);
    matrixMultiplication(scaleMatrix, resultMatrix);
    matrixMultiplication(shearingMatrix, resultMatrix);

    matrixInversion(rotateMatrix);
    matrixInversion(scaleMatrix);
    matrixInversion(shearingMatrix);

    int moveX = (height / 2) + moveMatrix[0][2] * 2;
    int moveY = (width / 2) + moveMatrix[1][2];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int x = floor((i - moveX) * resultMatrix[0][0] + (j - moveY) * resultMatrix[0][1] + 175);
            int y = floor((i - moveX) * resultMatrix[1][0] + (j - moveY) * resultMatrix[1][1] + 175);

            pix = img->scanLine(i);
            pix2 = pikachu->scanLine(x);

            if (x >= 0 && x < imageSize && y >= 0 && y < imageSize)
            {
                pix[4*j] = pix2[4*y];
                pix[4*j + 1] = pix2[4*y + 1];
                pix[4*j + 2] = pix2[4*y + 2];
            }
        }
    }
    update();
}

// operacje macierzy przeksztalcen
//  x'   1 0 tx   x
//  y' = 0 1 0  * y
//  1    0 0 1    1
void Ekran::on_moveXSlider_valueChanged(int value)
{
    moveMatrix[0][2] = value;
    QString qstr = QString::number(value);
    ui->moveXLabel->setText("Przesuniecie X = " + qstr);
    drawImage();
}

// x'   1 0 0     x
// y' = 0 1 ty  * y
// 1    0 0 1     1
void Ekran::on_moveYSlider_valueChanged(int value)
{
    moveMatrix[1][2] = value;
    QString qstr = QString::number(value);
    ui->moveYLabel->setText("Przesuniecie Y = " + qstr);
    drawImage();
}

// x'   cos -sin 0    x
// y' = sin cos    *  y
// 1    0    0   1    1
void Ekran::on_spinSlider_valueChanged(int value)
{
    double radian = (value * M_PI) / 180;
    rotateMatrix[0][0] = qCos(radian);
    rotateMatrix[0][1] = -qSin(radian);
    rotateMatrix[1][0] = qSin(radian);
    rotateMatrix[1][1] = qCos(radian);
    QString qstr = QString::number(value);
    ui->spinLabel->setText("Obrot = " + qstr);
    drawImage();
}

void Ekran::on_scaleXSlider_valueChanged(int value)
{
    scaleMatrix[0][0] = (double)value / 100;
    QString qstr = QString::number(value);
    ui->scaleXLabel->setText("Skalowanie X = " + qstr);
    drawImage();
}

// x'   sx 0 0    x
// y' = 0 sy 0  * y
// 1    0 0  1    1
void Ekran::on_scaleYSlider_valueChanged(int value)
{
    if(scaleBoth == 0)
    {
        scaleMatrix[1][1] = (double)value / 100;
        QString qstr = QString::number(value);
        ui->scaleYLabel->setText("Scalowanie Y = " + qstr);
        drawImage();
    }
    else
    {
        scaleMatrix[0][0] = (double)value / 100;
        scaleMatrix[1][1] = (double)value / 100;
        QString qstr = QString::number(value);
        ui->scaleXLabel->setText("Skalowanie X = " + qstr);
        ui->scaleYLabel->setText("Scalowanie Y = " + qstr);
        drawImage();
    }
}

void Ekran::on_checkBox_stateChanged(int arg1)
{
    scaleBoth = arg1;
}

// x'   1 shx  0    x
// y' = 0  1   0 *  y
// 1    0  0   1    1
void Ekran::on_shearingXSlider_valueChanged(int value)
{
    shearingMatrix[0][1] = (double)value / 100;
    QString qstr = QString::number(value);
    ui->shearingXLabel->setText("Pochylenie X = " + qstr);
    drawImage();
}

// x'   1    0   0    x
// y' = shy  1   0 *  y
// 1    0    0   1    1
void Ekran::on_shearingYSlider_valueChanged(int value)
{
    shearingMatrix[1][0] = (double)value / 100;
    QString qstr = QString::number(value);
    ui->shearingYLabel->setText("Pochylenie Y = " + qstr);
    drawImage();
}

void Ekran::on_resetButton_clicked()
{
    setImage();
}

void Ekran::on_loadButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Wybierz obraz", QDir::currentPath(),
            "Obrazy (*.png *.jpg *.jpeg *.bmp *.gif)");

    if (QString::compare(filename, QString()) != 0)
    {
        pikachu->load(filename);
        imageSize = pikachu->width();
        setImage();
    }
}

void Ekran::on_exitButton_clicked()
{
    qApp->quit();
}
