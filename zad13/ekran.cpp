#include "ekran.h"
#include "ui_ekran.h"

#include <QPainter>
#include <vector>

Ekran::Ekran(QWidget *parent)
            : QWidget(parent),
              ui(new Ui::Ekran)
{
    ui->setupUi(this);

    width = ui->frame->width();
    height = ui->frame->height();
    beginX = ui->frame->x();
    beginY = ui->frame->y();

    img = new QImage(width, height, QImage::Format_RGB32);
    source = new QImage(":tekstura.jpg");
    setConnections();
    drawImage();
}

Ekran::~Ekran()
{
    delete ui;
}

void Ekran::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(beginX, beginY, *img);
}

void Ekran::fillWhite(QImage *img)
{
    uchar *ptr;
    for (int i = 0; i < height; i++)
    {
        ptr = img->scanLine(i);
        for (int j = 0; j < width; j++)
        {
            ptr[4*j] = 255;
            ptr[4*j+1] = 255;
            ptr[4*j+2] = 255;
        }
    }
}

void Ekran::drawPixel(int x, int y, int r, int g, int b)
{
    if ((x >= 0) && (y >= 0) && (x < width) && (y < height))
    {
        uchar *ptr;
        ptr = img->scanLine(y);
        ptr[4*x] = b;
        ptr[4*x+1] = g;
        ptr[4*x+2] = r;
    }
}

void Ekran::drawPoint(point a)
{
    int x = a.x, y = a.y;
    for(int i = x - 3; i < x + 3; i++)
    {
        for(int j = y - 3; j < y + 3; j++)
        {
            drawPixel(i, j, 255, 0, 0);
        }
    }
}

void Ekran::drawLine(int x1, int y1, int x2, int y2)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    int k;
    double x = x1;
    double y = y1;

    if (abs(dx) > abs(dy))
        k = abs(dx);
    else
        k = abs(dy);

    if (k != 0)
    {
        double mx = dx / (double)k;
        double my = dy / (double)k;

        drawPixel(round(x), round(y), 0, 0, 0);
        for (int i = 0; i < k; i++)
        {
            x += mx;
            y += my;
            drawPixel(round(x), round(y), 0, 0, 0);
        }
    }
}

int Ekran::round(int a)
{
    return (int)(a + 0.5);
}

void Ekran::drawImage()
{
    int halfImgX = width / 2, halfImgY = height / 2, length = 100,
        translateX, translateY, translateZ;
    double scaleX, scaleY, scaleZ, rotateX, rotateY, rotateZ,
           shearX, shearY, shearZ, tmpX, tmpY, tmpZ;

    // pobranie wszystkich wartosci ze sliderow
    translateX = ui->translationX->value();
    translateY = ui->translationY->value();
    translateZ = ui->translationZ->value();
    scaleX = ui->scalingX->value() / 100.0;
    scaleY = ui->scalingY->value() / 100.0;
    scaleZ = ui->scalingZ->value() / 100.0;
    rotateX = ui->rotationX->value() / 10000.0 - 1;
    rotateY = ui->rotationY->value() / 10000.0 - 1;
    rotateZ = ui->rotationZ->value() / 10000.0 - 1;
    shearX = ui->shearingX->value();
    shearY = ui->shearingY->value();
    shearZ = ui->shearingZ->value();

    fillWhite(img); // pokolorowanie tla na bialo

    /*
    // wstawianie punktow do wektora - przod bryly
    cubePoints.push_back({halfImgX - (length / 2), halfImgY - (length / 2), 0}); // lewy gorny
    cubePoints.push_back({halfImgX + (length / 2), halfImgY - (length / 2), 0}); // prawy gorny
    cubePoints.push_back({halfImgX + (length / 2), halfImgY + (length / 2), 0}); // prawy dolny
    cubePoints.push_back({halfImgX - (length / 2), halfImgY + (length / 2), 0}); // lewy dolny

    // tyl bryly
    cubePoints.push_back({halfImgX - (length / 2), halfImgY - (length / 2), length}); // lewy gorny
    cubePoints.push_back({halfImgX + (length / 2), halfImgY - (length / 2), length}); // prawy gorny
    cubePoints.push_back({halfImgX + (length / 2), halfImgY + (length / 2), length}); // prawy downy
    cubePoints.push_back({halfImgX - (length / 2), halfImgY + (length / 2), length}); // lewy dolny*/

    // wstawianie punktow do wektora - przod bryly
    cubePoints.push_back({-(length / 2), -(length / 2), 0}); // lewy gorny
    cubePoints.push_back({(length / 2), -(length / 2), 0}); // prawy gorny
    cubePoints.push_back({(length / 2), +(length / 2), 0}); // prawy dolny
    cubePoints.push_back({-(length / 2), +(length / 2), 0}); // lewy dolny

    // tyl bryly
    cubePoints.push_back({-(length / 2), -(length / 2), length}); // lewy gorny
    cubePoints.push_back({(length / 2), -(length / 2), length}); // prawy gorny
    cubePoints.push_back({(length / 2), +(length / 2), length}); // prawy dolny
    cubePoints.push_back({-(length / 2), +(length / 2), length}); // lewy dolny

    // wszystkie przeksztalcenia
    for (int i = 0; i < (int)cubePoints.size(); i++)
    {
        // wysrodkowanie pozycji bryly
        cubePoints[i].x = cubePoints[i].x - halfImgX;
        cubePoints[i].y = cubePoints[i].y - halfImgY;

        if (rotateX != 0) // rotacja na osi X
        {
            tmpY = ((cubePoints[i].y * cos(rotateX)) - (cubePoints[i].z * sin(rotateX)));
            tmpZ = ((cubePoints[i].y * sin(rotateX)) + (cubePoints[i].z * cos(rotateX)));
            cubePoints[i].y = tmpY;
            cubePoints[i].z = tmpZ;
        }
        if (rotateY != 0) // rotacja na osi Y
        {
            tmpX = ((cubePoints[i].x * cos(rotateY)) + (cubePoints[i].z * sin(rotateY)));
            tmpZ = (((-1 * cubePoints[i].x) * sin(rotateY)) + (cubePoints[i].z * cos(rotateY)));
            cubePoints[i].x = tmpX;
            cubePoints[i].z = tmpZ;
        }
        if (rotateZ != 0) // rotacja na osi Z
        {
            tmpX = ((cubePoints[i].x * cos(rotateZ)) - (cubePoints[i].y * sin(rotateZ)));
            tmpY = ((cubePoints[i].x * sin(rotateZ)) + (cubePoints[i].y * cos(rotateZ)));
            cubePoints[i].x = tmpX;
            cubePoints[i].y = tmpY;
        }

        if (shearX != 0) // pochylenie X
        {
            tmpX = cubePoints[i].x + (cubePoints[i].z * (double)shearX / 100.0);
            cubePoints[i].x = (double)tmpX;
        }
        if (shearY != 0) // pochylenie Y
        {
            tmpY = cubePoints[i].y + (cubePoints[i].z * (double)shearY / 100.0);
            cubePoints[i].y = (double)tmpY;
        }
        if (shearZ != 0) // pochylenie Z
        {
            tmpZ = cubePoints[i].z + (cubePoints[i].z * (double)shearZ / 100.0);
            cubePoints[i].z = (double)tmpZ;
        }

        // skalowanie + translacja
        if (allScale == false)
        {
            if (scaleX != 0)
                cubePoints[i].x = (cubePoints[i].x * scaleX) + halfImgX + translateX;
            if (scaleY != 0)
                cubePoints[i].y = (cubePoints[i].y * scaleY) + halfImgY + translateY;
            if (scaleZ != 0)
                cubePoints[i].z = (cubePoints[i].z * scaleZ) + translateZ;
        }
        else if (scaleX != 0)
        {
            cubePoints[i].x = (cubePoints[i].x * scaleX) + halfImgX + translateX;
            cubePoints[i].y = (cubePoints[i].y * scaleX) + halfImgY + translateY;
            cubePoints[i].z = (cubePoints[i].z * scaleX) + translateZ;
        }

        // konwersja z 3D na 2D
        //cubePoints[i].x = (cubePoints[i].x / ((cubePoints[i].z / 1000.0) + 1.0));
        //cubePoints[i].y = (cubePoints[i].y / ((cubePoints[i].z / 1000.0) + 1.0));

        cubePoints[i].x = (cubePoints[i].x / ((cubePoints[i].z / 1000.0) + 1.0)) + halfImgX;
        cubePoints[i].y = (cubePoints[i].y / ((cubePoints[i].z / 1000.0) + 1.0)) + halfImgY;

        // rysowanie czerwonych punktow
        if (wallVisibility == false && texturing == false)
            drawPoint(cubePoints[i]);
    }

    // rysowanie bryly
    if (wallVisibility == false) // bool z checkboxa
    {
        // gorna sciana
        drawLine(cubePoints[0].x, cubePoints[0].y, cubePoints[1].x, cubePoints[1].y);
        drawLine(cubePoints[0].x, cubePoints[0].y, cubePoints[4].x, cubePoints[4].y);
        drawLine(cubePoints[4].x, cubePoints[4].y, cubePoints[5].x, cubePoints[5].y);
        drawLine(cubePoints[5].x, cubePoints[5].y, cubePoints[1].x, cubePoints[1].y);

        // dolna sciana
        drawLine(cubePoints[2].x, cubePoints[2].y, cubePoints[3].x, cubePoints[3].y);
        drawLine(cubePoints[2].x, cubePoints[2].y, cubePoints[6].x, cubePoints[6].y);
        drawLine(cubePoints[3].x, cubePoints[3].y, cubePoints[7].x, cubePoints[7].y);
        drawLine(cubePoints[6].x, cubePoints[6].y, cubePoints[7].x, cubePoints[7].y);

        // sciany boczne
        drawLine(cubePoints[0].x, cubePoints[0].y, cubePoints[3].x, cubePoints[3].y);
        drawLine(cubePoints[1].x, cubePoints[1].y, cubePoints[2].x, cubePoints[2].y);
        drawLine(cubePoints[4].x, cubePoints[4].y, cubePoints[7].x, cubePoints[7].y);
        drawLine(cubePoints[5].x, cubePoints[5].y, cubePoints[6].x, cubePoints[6].y);
    }
    else
    {
        // wspolrzedne do tekstury
        point srcCoords[] = { {0, 0, 0}, {400, 0, 0}, {0, 400, 0}, {400, 400, 0} };

        if (isVisible(cubePoints[4].x, cubePoints[4].y, cubePoints[0].x, cubePoints[0].y, cubePoints[3].x, cubePoints[3].y)) // lewa sciana
        {
            if (texturing == true) // bool z checkboxa
            {
                triangleTexturing(cubePoints[4], cubePoints[0], cubePoints[7], srcCoords[0], srcCoords[1], srcCoords[2], img, source);
                triangleTexturing(cubePoints[3], cubePoints[7], cubePoints[0], srcCoords[3], srcCoords[2], srcCoords[1], img, source);
            }
            drawLine(cubePoints[0].x, cubePoints[0].y, cubePoints[4].x, cubePoints[4].y);
            drawLine(cubePoints[0].x, cubePoints[0].y, cubePoints[3].x, cubePoints[3].y);
            drawLine(cubePoints[7].x, cubePoints[7].y, cubePoints[3].x, cubePoints[3].y);
            drawLine(cubePoints[7].x, cubePoints[7].y, cubePoints[4].x, cubePoints[4].y);
        }
        if (isVisible(cubePoints[1].x, cubePoints[1].y, cubePoints[5].x, cubePoints[5].y, cubePoints[6].x, cubePoints[6].y)) // prawa sciana
        {
            if (texturing == true)
            {
                triangleTexturing(cubePoints[1], cubePoints[5], cubePoints[2], srcCoords[0], srcCoords[1], srcCoords[2], img, source);
                triangleTexturing(cubePoints[6], cubePoints[2], cubePoints[5], srcCoords[3], srcCoords[2], srcCoords[1], img, source);
            }
            drawLine(cubePoints[1].x, cubePoints[1].y, cubePoints[2].x, cubePoints[2].y);
            drawLine(cubePoints[1].x, cubePoints[1].y, cubePoints[5].x, cubePoints[5].y);
            drawLine(cubePoints[6].x, cubePoints[6].y, cubePoints[2].x, cubePoints[2].y);
            drawLine(cubePoints[6].x, cubePoints[6].y, cubePoints[5].x, cubePoints[5].y);
        }
        if (isVisible(cubePoints[0].x, cubePoints[0].y, cubePoints[4].x, cubePoints[4].y, cubePoints[1].x, cubePoints[1].y)) // gorna sciana
        {
            if (texturing == true)
            {
                triangleTexturing(cubePoints[4], cubePoints[5], cubePoints[0], srcCoords[0], srcCoords[1], srcCoords[2], img, source);
                triangleTexturing(cubePoints[1], cubePoints[0], cubePoints[5], srcCoords[3], srcCoords[2], srcCoords[1], img, source);
            }
            drawLine(cubePoints[0].x, cubePoints[0].y, cubePoints[4].x, cubePoints[4].y);
            drawLine(cubePoints[0].x, cubePoints[0].y, cubePoints[1].x, cubePoints[1].y);
            drawLine(cubePoints[1].x, cubePoints[1].y, cubePoints[5].x, cubePoints[5].y);
            drawLine(cubePoints[4].x, cubePoints[4].y, cubePoints[5].x, cubePoints[5].y);
        }
        if (isVisible(cubePoints[3].x, cubePoints[3].y, cubePoints[2].x, cubePoints[2].y, cubePoints[7].x, cubePoints[7].y)) // dolna sciana
        {
            if (texturing == true)
            {
                triangleTexturing(cubePoints[3], cubePoints[2], cubePoints[7], srcCoords[0], srcCoords[1], srcCoords[2], img, source);
                triangleTexturing(cubePoints[6], cubePoints[7], cubePoints[2], srcCoords[3], srcCoords[2], srcCoords[1], img, source);
            }
            drawLine(cubePoints[3].x, cubePoints[3].y, cubePoints[2].x, cubePoints[2].y);
            drawLine(cubePoints[3].x, cubePoints[3].y, cubePoints[7].x, cubePoints[7].y);
            drawLine(cubePoints[6].x, cubePoints[6].y, cubePoints[2].x, cubePoints[2].y);
            drawLine(cubePoints[6].x, cubePoints[6].y, cubePoints[7].x, cubePoints[7].y);
        }
        if (isVisible(cubePoints[0].x, cubePoints[0].y, cubePoints[1].x, cubePoints[1].y, cubePoints[3].x, cubePoints[3].y)) // przednia sciana
        {
            if (texturing == true)
            {
                triangleTexturing(cubePoints[0], cubePoints[1], cubePoints[3], srcCoords[0], srcCoords[1], srcCoords[2], img, source);
                triangleTexturing(cubePoints[2], cubePoints[3], cubePoints[1], srcCoords[3], srcCoords[2], srcCoords[1], img, source);
            }
            drawLine(cubePoints[0].x, cubePoints[0].y, cubePoints[1].x, cubePoints[1].y);
            drawLine(cubePoints[0].x, cubePoints[0].y, cubePoints[3].x, cubePoints[3].y);
            drawLine(cubePoints[2].x, cubePoints[2].y, cubePoints[3].x, cubePoints[3].y);
            drawLine(cubePoints[2].x, cubePoints[2].y, cubePoints[1].x, cubePoints[1].y);
        }
        if (isVisible(cubePoints[4].x, cubePoints[4].y, cubePoints[7].x, cubePoints[7].y, cubePoints[5].x, cubePoints[5].y)) // tylna sciana
        {
            if (texturing == true)
            {
                triangleTexturing(cubePoints[7], cubePoints[6], cubePoints[4], srcCoords[0], srcCoords[1], srcCoords[2], img, source);
                triangleTexturing(cubePoints[5], cubePoints[4], cubePoints[6], srcCoords[3], srcCoords[2], srcCoords[1], img, source);
            }
            drawLine(cubePoints[4].x, cubePoints[4].y, cubePoints[7].x, cubePoints[7].y);
            drawLine(cubePoints[4].x, cubePoints[4].y, cubePoints[5].x, cubePoints[5].y);
            drawLine(cubePoints[6].x, cubePoints[6].y, cubePoints[5].x, cubePoints[5].y);
            drawLine(cubePoints[6].x, cubePoints[6].y, cubePoints[7].x, cubePoints[7].y);
        }
    }
    cubePoints.clear();
    update();
}

bool Ekran::isVisible(int x1, int x2, int y1, int y2, int z1, int z2)
{
    return ( ((x1 - y1) * (z2 - y2)) - ((z1 - y1) * (x2 - y2)) ) < 0;
}

void Ekran::triangleTexturing(point a1, point a2, point a3, point b1, point b2, point b3, QImage *dest, QImage *source)
{
    double xd, yd, u, v, w;
    int xi, yi, tmpX, tmpY;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // wyliczam wspolrzedne barycentryczne
            v = (double)((i - a1.x) * (a3.y - a1.y) - ((j - a1.y) * (a3.x - a1.x))) /
                (((a2.x - a1.x) * (a3.y - a1.y)) - ((a2.y - a1.y) * (a3.x - a1.x)));
            w = (double)(((a2.x - a1.x) * (j - a1.y)) - ((i - a1.x) * (a2.y - a1.y))) /
                (((a2.x - a1.x) * (a3.y - a1.y)) - ((a2.y - a1.y) * (a3.x - a1.x)));
            u = (double)(1.0) - (v + w);

            // jezeli punkt znajduje sie w trojkacie to go zamalowuje
            if (!(v < 0 || v > 1 || u < 0 || u > 1 || w < 0 || w > 1))
            {
                uchar *ptr, *ptr2;
                ptr = source->bits();

                xd = u * b1.x + v * b2.x + w * b3.x;
                yd = u * b1.y + v * b2.y + w * b3.y;
                xi = (int)xd;
                yi = (int)yd;

                tmpX = xi + 1;
                tmpY = yi + 1;
                xd -= xi;
                yd -= yi;

                ptr2 = dest->scanLine(j);
                ptr2[4*i] = ((1 - xd) * ((1 - yd) * ptr[width*4*yi + 4*xi] + yd * ptr[width*4*tmpY + 4*xi])
                        + xd * ((1 - yd) * ptr[width*4*yi + 4*tmpX] + yd * ptr[width*4*tmpY + 4*tmpX]));
                ptr2[4*i+1] = ((1 - xd) * ((1 - yd) * ptr[width*4*yi + 4*xi+1] + yd * ptr[width*4*tmpY + 4*xi+1])
                        + xd * ((1 - yd) * ptr[width*4*yi + 4*tmpX+1] + yd * ptr[width*4*tmpY + 4*tmpX+1]));
                ptr2[4*i+2] = ((1 - xd) * ((1 - yd) * ptr[width*4*yi + 4*xi+2] + yd * ptr[width*4*tmpY + 4*xi+2])
                        + xd * ((1 - yd) * ptr[width*4*yi + 4*tmpX+2] + yd * ptr[width*4*tmpY + 4*tmpX+2]));
                update();
            }
        }
    }
}

void Ekran::setConnections()
{
    connect(ui->translationX, SIGNAL(valueChanged(int)), this, SLOT(drawImage()));
    connect(ui->translationY, SIGNAL(valueChanged(int)), this, SLOT(drawImage()));
    connect(ui->translationZ, SIGNAL(valueChanged(int)), this, SLOT(drawImage()));
    connect(ui->scalingX, SIGNAL(valueChanged(int)), this, SLOT(drawImage()));
    connect(ui->scalingY, SIGNAL(valueChanged(int)), this, SLOT(drawImage()));
    connect(ui->scalingZ, SIGNAL(valueChanged(int)), this, SLOT(drawImage()));
    connect(ui->rotationX, SIGNAL(valueChanged(int)), this, SLOT(drawImage()));
    connect(ui->rotationY, SIGNAL(valueChanged(int)), this, SLOT(drawImage()));
    connect(ui->rotationZ, SIGNAL(valueChanged(int)), this, SLOT(drawImage()));
    connect(ui->shearingX, SIGNAL(valueChanged(int)), this, SLOT(drawImage()));
    connect(ui->shearingY, SIGNAL(valueChanged(int)), this, SLOT(drawImage()));
    connect(ui->shearingZ, SIGNAL(valueChanged(int)), this, SLOT(drawImage()));
}

void Ekran::on_resetButton_clicked()
{
    fillWhite(img);
    ui->translationX->setValue(0);
    ui->translationY->setValue(0);
    ui->translationZ->setValue(0);
    ui->scalingX->setValue(100);
    ui->scalingY->setValue(100);
    ui->scalingZ->setValue(90);
    ui->rotationX->setValue(0);
    ui->rotationY->setValue(0);
    ui->rotationZ->setValue(0);
    ui->shearingX->setValue(0);
    ui->shearingY->setValue(0);
    ui->shearingZ->setValue(0);
    drawImage();
}

void Ekran::on_allScaleCheckBox_clicked(bool checked)
{
    allScale = checked;
}

void Ekran::on_wallVisibilityCheckBox_clicked(bool checked)
{
    wallVisibility = checked;
    drawImage();
}

void Ekran::on_texturingCheckBox_clicked(bool checked)
{
    texturing = checked;
    drawImage();
}

void Ekran::on_quitButton_clicked()
{
    qApp->quit();
}
