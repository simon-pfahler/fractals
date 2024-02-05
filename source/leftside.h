#ifndef LEFTSIDE_H
#define LEFTSIDE_H

#include <QWidget>
#include <QGraphicsView>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QGridLayout>
#include <QFileDialog>

#include <complex>

#include "mandelbrot.h"
#include "julia.h"

class LeftSide : public QWidget
{
    Q_OBJECT
public:
    explicit LeftSide(QWidget *parent = nullptr);

signals:
    void update_z0(std::complex<double> const &newZ0);
    void progress_update(unsigned int const &progress);

private slots:
    void save_image();

public slots:
    void update_iterations(unsigned int const &newIterations);
    void update_exponent(double const &newExponent);
    void update_size();
    void reset_zoom();
    void render_smooth();

private:

    //member data of the left side
    Mandelbrot *mImage;
    QPushButton *mSave;
    QCheckBox *mSaveCross;
    QLabel *mIterator;

};

#endif // LEFTSIDE_H
