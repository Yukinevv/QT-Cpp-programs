#ifndef REZULTAT_H
#define REZULTAT_H

#include <QWidget>

class Rezultat : public QWidget
{
    Q_OBJECT
public:    
    explicit Rezultat(QWidget *parent = nullptr);

    QImage *img;

    void clearWidget();

    void paintEvent(QPaintEvent *);
};

#endif // REZULTAT_H
