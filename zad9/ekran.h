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

    QImage img, img2, img3;
    int width = 500, height = 500;

    void drawPixel(int x, int y, int r, int g, int b);
    bool pixelWhiteOrBlack(int x, int y);
    void pictureTranslate();
    void erosion();
    void dilation();
protected:
    void paintEvent(QPaintEvent *);
signals:

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::Ekran *ui;
};

#endif // EKRAN_H
