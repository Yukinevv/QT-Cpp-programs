#include "ekran.h"
#include "ui_ekran.h"

#include <QMouseEvent>
#include <QPainter>

Ekran::Ekran(QWidget *parent)
    : QWidget{parent},
      im(1250, 500, QImage::Format_RGB32),
      tmp(1250, 500, QImage::Format_RGB32),
      ui(new Ui::Ekran)
{
    ui->setupUi(this);
    im.fill(0);
}

Ekran::~Ekran()
{
    delete ui;
}

void Ekran::scanLine(std::vector<Punkt> P)
{
    if((int)P.size() > 0)
    {
        float yMin = P[0].y;
        float yMax = P[0].y;

        for(int i = 0; i < (int)P.size(); i++)
        {
            if(yMin > P[i].y)
                yMin = P[i].y;

            if(yMax < P[i].y)
                yMax = P[i].y;
        }

        std::vector<float> crossPoints;
        float crossPoint;
        int l_cross = 0;

        for(int y = yMin + 1; y < yMax; y++)
        {
            for(int i = 0; i < (int)P.size() - 1; i++)
            {
                if(((P[i].y >= y && P[i+1].y <= y) || (P[i].y <= y && P[i+1].y >= y)) && (P[i].y != P[i+1].y))
                {
                    crossPoint = P[i].x + (y - P[i].y) * ( (P[i+1].x - P[i].x) / (P[i+1].y - P[i].y) );

                    if (l_cross == i)
                    {
                        if((P[i].y > P[i+1].y && P[i].y > P[i-1].y) || (P[i].y < P[i+1].y && P[i].y < P[i-1].y))
                        {
                            crossPoints.push_back(crossPoint);
                        }
                    }
                    else
                    {
                        crossPoints.push_back(crossPoint);
                    }
                    l_cross = i + 1;
                }
            }

            std::sort(crossPoints.begin(), crossPoints.end());

            for(int i = 0; i <= (int)crossPoints.size() - 1; i+=2)
            {
                for(int n = crossPoints[i]+1; n < crossPoints[i+1]; n++)
                {
                    drawPixel(n, y, R, G, B);
                }
            }
            crossPoints.clear();
        }

        drawShape(P);
        update();
    }
}

void Ekran::drawShape(std::vector<Punkt> P)
{
    R = G = B = 255;
    for(int i = 1; i < (int)P.size(); i++)
    {
        drawLine(P[i-1].x, P[i-1].y, P[i].x, P[i].y);
    }
}

//Events
void Ekran::mousePressEvent(QMouseEvent *e)
{
    m1 = e->pos();
    outside_the_area = false;

    if(m1.x() >= 0 && m1.x() < im.width() && m1.y() >= 0 && m1.y() < im.height())
        qDebug("x1: %d   y1: %d", m1.x(), m1.y());
    else
    {
        qDebug("Rysujesz poza obszarem");
        outside_the_area = true;
    }

    if(m1.x() < 0 || m1.x() >= im.width() || m1.y() < 0 || m1.y() >= im.height())
        return;
    else if(buttonStatus == 1)
    {
        drawPixel(m1.x(), m1.y(), R, G, B);
    }
    else if(buttonStatus == 3)
    {
        floodFill(m1.x(), m1.y(), getColor(m1.x(), m1.y()), R, G, B);
    }
    else if(buttonStatus == 4)
    {
        scanLine(scanPoints);
    }
    else if(buttonStatus == 5) // rysowanie ksztaltu poprzez dodawanie punktow
    {
        R = G = B = 255;
        bool last_point = false;
        Punkt Q;
        if((int)scanPoints.size() > 0)
        {
            if((sqrt(pow(scanPoints[licznik].x - scanPoints[0].x, 2) + pow(scanPoints[licznik].y - scanPoints[0].y, 2)) < 30) && (licznik > 0))
            {
                drawLine(scanPoints[licznik].x, scanPoints[licznik].y, scanPoints[0].x, scanPoints[0].y);
                scanPoints.push_back(scanPoints[0]);
                last_point = true;
            }
            else
            {
                drawLine(scanPoints[licznik].x, scanPoints[licznik].y, m1.x(), m1.y());
                licznik++;
            }
        }

        if(licznik == 0)
            drawPixel(m1.x(), m1.y(), R, G, B);

        if(last_point == false)
        {
            Q.x = m1.x();
            Q.y = m1.y();
            scanPoints.push_back(Q);
        }
    }
    else if(buttonStatus == 6) // ksztalt na sztywno
    {
        R = G = B = 255;
        scanPoints.push_back({696, 391});
        scanPoints.push_back({804, 140});
        scanPoints.push_back({982, 346});
        scanPoints.push_back({1100, 54});
        scanPoints.push_back({655, 32});
        scanPoints.push_back({640, 172});
        scanPoints.push_back({357, 31});
        scanPoints.push_back({425, 162});
        scanPoints.push_back({124, 58});
        scanPoints.push_back({223, 384});
        scanPoints.push_back({523, 186});
        scanPoints.push_back({505, 186});
        scanPoints.push_back({497, 367});
        scanPoints.push_back({665, 249});
        scanPoints.push_back({696, 391});

        drawShape(scanPoints);
    }

    tmp = im;
}

void Ekran::mouseMoveEvent(QMouseEvent *e)
{
    m2 = e->pos();
    im = tmp;

    if(m1.x() < 0 || m1.x() >= im.width() || m1.y() < 0 || m1.y() >= im.height())
        return;
    else if(buttonStatus == 1)
    {
        drawLine(m1.x(), m1.y(), m2.x(), m2.y());
    }
    else if(buttonStatus == 2)
    {
        drawCircle(m1.x(), m1.y(), m2.x(), m2.y());
    }
}

void Ekran::mouseReleaseEvent(QMouseEvent *e)
{
    m2 = e->pos();

    if(m2.x() >= 0 && m2.x() < im.width() && m2.y() >= 0 && m2.y() < im.height() && outside_the_area == false)
        qDebug("x2: %d   y2: %d", m2.x(), m2.y());
    else if(outside_the_area == true)
        return;
    else
        qDebug("Rysujesz poza obszarem");
}

void Ekran::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(0, 0, width(), height()/2, Qt::gray);
    p.fillRect(0, height()/2, width(), height()/2, Qt::gray);

    p.drawImage(0, 0, im);
}

//Metody z UI
void Ekran::on_cleanButton_clicked()
{
    im.fill(0);
    update();

    scanPoints.clear();
    licznik = 0;
}

void Ekran::on_quitButton_clicked()
{
    qApp->quit();
}

void Ekran::on_randomColorButton_clicked()
{
    R = rand() % 255 + 1;
    G = rand() % 255 + 1;
    B = rand() % 255 + 1;
    qDebug("Wylosowane kolory to: red = %d green = %d blue = %d", R, G, B);
}

void Ekran::on_lineButton_clicked()
{
    buttonStatus = 1;
}

void Ekran::on_circleButton_clicked()
{
    buttonStatus = 2;
}

void Ekran::on_floodFillButton_clicked()
{
    buttonStatus = 3;
}

void Ekran::on_scanLineButton_clicked()
{
    buttonStatus = 4;
}

void Ekran::on_addPointButton_clicked()
{
    buttonStatus = 5;
}

void Ekran::on_drawPolygonButton_clicked()
{
    buttonStatus = 6;
}

//Metody do rysowania z poprzednich zadan

void Ekran::drawPixel(int x, int y, int r, int g, int b)
{
    if(x < 0 || x >= im.width() || y < 0 || y >= im.height())
        return;

    uchar *pix = im.scanLine(y);
    pix[4*x] = r;
    pix[4*x + 1] = g;
    pix[4*x + 2] = b;
    update();
}

void Ekran::drawLine(int x1, int y1, int x2, int y2)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    int k;
    float x = x1;
    float y = y1;

    if(abs(dx) > abs(dy))
        k = abs(dx);
    else
        k = abs(dy);

    if(k != 0)
    {
        float mx = dx / (float)k;
        float my = dy / (float)k;

        drawPixel(Round(x), Round(y), R, G, B);
        for(int i = 0; i < k; i++)
        {
            x += mx;
            y += my;
            drawPixel(Round(x), Round(y), R, G, B);
        }
    }
}

void Ekran::drawCircle(int x1, int y1, int x2, int y2)
{
    float R = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    float y;

    for(int x = 0; x <= (sqrt(2)/2)*R; x++)
    {
        y = sqrt(pow(R, 2) - pow(x, 2));
        drawPixel(x + x1, Round(y) + y1, R, G, B);
        drawPixel(x + x1, -1*Round(y) + y1, R, G, B);
        drawPixel(-x + x1, Round(y) + y1, R, G, B);
        drawPixel(-x + x1, -1*Round(y) + y1, R, G, B);
        drawPixel(Round(y) + x1, x + y1, R, G, B);
        drawPixel(Round(y) + x1, -x + y1, R, G, B);
        drawPixel(-1*Round(y) + x1, x + y1, R, G, B);
        drawPixel(-1*Round(y) + x1, -x + y1, R, G, B);
    }
}

QRgb Ekran::getColor(int x, int y)
{
    uchar *pix = im.scanLine(y);
    return qRgb(pix[4*x], pix[4*x + 1], pix[4*x + 2]);
}

void Ekran::floodFill(int x, int y, QRgb color, int newColor1, int newColor2, int newColor3)
{
    QPoint P;
    if(qRed(color) != newColor1 || qGreen(color) != newColor2 || qBlue(color) != newColor3)
    {
        if(x >= 0 && x < im.width() && y >= 0 && y < im.height())
        {
            pointStack.push({x, y});

            while(!pointStack.empty())
            {
                P = pointStack.top();
                x = P.x();
                y = P.y();
                pointStack.pop();
                if(getColor(x, y) == color)
                {
                    drawPixel(x, y, newColor1, newColor2, newColor3);
                    if(x-1 >= 0)
                        pointStack.push({x-1, y});
                    if(x+1 < im.width())
                        pointStack.push({x+1, y});
                    if(y-1 >= 0)
                        pointStack.push({x, y-1});
                    if(y+1 < im.height())
                        pointStack.push({x, y+1});
                }
            }
            update();
        }
    }
}

int Ekran::Round(int a)
{
    return (int)(a + 0.5);
}
