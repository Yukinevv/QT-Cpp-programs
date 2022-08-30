#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ekran.h"
#include "rezultat.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Ekran *ekran;
    Rezultat *rezultat;
    QImage *img1, *img2;
    Point triangle1[3], triangle2[3];
    int frameAmount = 10;

    void barycentricCoordinates(Point p, int n);
    void triangleTexturing(int n);

public slots:
    void paintEvent(QPaintEvent *event);

private slots:
    void on_changeImageSlider_valueChanged(int value);

    void on_spinBox_valueChanged(int arg1);

    void on_changeFrameSlider_valueChanged(int value);

    void on_quitButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
