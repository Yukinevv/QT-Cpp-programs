#include "rezultat.h"

#include <QPainter>

Rezultat::Rezultat(QWidget *parent)
                  : QWidget(parent)
{
    img = new QImage(500, 500, QImage::Format_RGB32);
    img->fill(*new QColor(0, 0, 0));
    this->setGeometry(500, 0, 500, 500);
}

void Rezultat::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawRect(0, 0, 500, 500);
    p.drawImage(0, 0, *img);
    update();
}

void Rezultat::clearWidget()
{
    for (int i = 0; i < 500; i++)
    {
        uchar *ptr = img->scanLine(i);
        for (int j = 0; j < 500; j++)
        {
            ptr[4*j] = 0;
            ptr[4*j + 1] = 0;
            ptr[4*j + 2] = 0;
        }
    }
}
