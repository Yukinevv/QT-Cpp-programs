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

    struct point
    {
        int x, y, z;
    };
    QImage *img, *source;
    std::vector<point> cubePoints;
    int width, height, beginX, beginY;
    bool wallVisibility = false, texturing = false, allScale = false;

    void setConnections();
    void fillWhite(QImage *img);
    void drawPixel(int x, int y, int r, int g, int b);
    void drawPoint(point a);
    void drawLine(int x1, int y1, int x2, int y2);
    int round(int a);
    void triangleTexturing(point a1, point a2, point a3, point b1, point b2, point b3, QImage *dest, QImage *source);
    bool isVisible(int x1, int x2, int y1, int y2, int z1, int z2);

protected:
    void paintEvent(QPaintEvent *);

signals:

private slots:
    void drawImage();

    void on_allScaleCheckBox_clicked(bool checked);

    void on_wallVisibilityCheckBox_clicked(bool checked);

    void on_texturingCheckBox_clicked(bool checked);

    void on_resetButton_clicked();

    void on_quitButton_clicked();

private:
    Ui::Ekran *ui;
};

#endif // EKRAN_H
