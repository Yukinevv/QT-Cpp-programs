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

    QImage *img, *images[3];
    int showPicture[3], hide[3], mode[3];

    void addModes();
    void fillWhite(uchar *pix);
    int lighten(int c1, int c2);
    int darken(int c1, int c2);  
    int multiply(int c1, int c2);
    int diff(int c1, int c2);
    void drawImage();

protected:
    void paintEvent(QPaintEvent *);
signals:

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_2_currentIndexChanged(int index);

    void on_comboBox_3_currentIndexChanged(int index);

    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_horizontalSlider_3_valueChanged(int value);

    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

    void on_checkBox_3_stateChanged(int arg1);

private:
    Ui::Ekran *ui;
    int height = 473, width = 500;
};

#endif // EKRAN_H
