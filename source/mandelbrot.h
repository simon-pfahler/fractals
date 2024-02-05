#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPixmap>

#include <complex>
#include <algorithm>

class Mandelbrot : public QGraphicsView
{

    Q_OBJECT

public:
    Mandelbrot(QWidget *parent);

    //setter & getters
    std::complex<double> &center();
    std::complex<double> const &center() const;
    double &deltareal();
    double const &deltareal() const;
    unsigned int &iterations();
    unsigned int const &iterations() const;
    double &exponent();
    double const &exponent() const;

    //render the new fractal
    void render(bool smooth = false);

    //react to a click on the image and to pressed keys
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    //remove & readd the cross
    void remove_cross();
    void add_cross();

signals:
    //a signal that gets sent when z0 has to be updated
    void update_z0(std::complex<double> const newZ0);

    //send a signal to update the progress bar, 0 <= progress <= 100
    void progress_update(double const &progress);

private:


    //update the position of the cross
    void update_cross();

    //complex number in the center
    std::complex<double> mCenter;

    //range of the real part that is depicted, the range of imag is calculated so it's to scale
    double mDeltareal;

    //number of iterations
    unsigned int mIterations;

    //exponent in the iterator
    double mExponent;

    //complex number of the cross
    std::complex<double> mCrossNumber;

    //QGraphicsScene that holds the fractal
    QGraphicsScene *mScene;

    //Two QGraphicsLineItems that hold the two strokes of the cross
    QGraphicsLineItem *mLine1;
    QGraphicsLineItem *mLine2;
};

#endif // MANDELBROT_H
