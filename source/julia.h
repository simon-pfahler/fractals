#ifndef JULIA_H
#define JULIA_H

#include <QGraphicsView>
#include <complex>

class Julia : public QGraphicsView
{
    Q_OBJECT

public:
    Julia(QWidget *parent);

    //setter & getters
    std::complex<double> &center();
    std::complex<double> const &center() const;
    std::complex<double> &z0();
    std::complex<double> const &z0() const;
    unsigned int &iterations();
    unsigned int const &iterations() const;
    double &deltareal();
    double const &deltareal() const;
    double &exponent();
    double const &exponent() const;


    //render the new fractal
    void render(bool smooth = false);

    //react to pressed keys
    void keyPressEvent(QKeyEvent *event) override;

signals:
    //send a signal to update the progress bar, 0 <= progress <= 100
    void progress_update(unsigned int const &progress);

private:

    //complex number in the center
    std::complex<double> mCenter;

    //the number that characterizes this specific Julia set (z_n+1=z_n^2+mZ0)
    std::complex<double> mZ0;

    //range of the real part that is depicted, the range of imag is calculated so it's to scale
    double mDeltareal;

    //number of iterations
    unsigned int mIterations;

    //exponent in the iterator
    double mExponent;

    //QGraphicsScene that holds the fractal
    QGraphicsScene *mScene;

};

#endif // JULIA_H
