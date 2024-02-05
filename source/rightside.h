#ifndef RIGHTSIDE_H
#define RIGHTSIDE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QFileDialog>

#include <complex>

#include "julia.h"

class RightSide : public QWidget
{
    Q_OBJECT
public:
    explicit RightSide(QWidget *parent = nullptr);

signals:
    void progress_update(unsigned int const &progress);

public slots:
    void update_z0(std::complex<double> const &newZ0);
    void save_image();
    void update_iterations(unsigned int const &newIterations);
    void update_exponent(double const &newExponent);
    void update_size();
    void reset_zoom();
    void render_smooth();

private:

    QString getIterationLabel();

    //member data of the left side
    Julia *mImage;
    QPushButton *mSave;
    QLabel *mIterator;    
};

#endif // RIGHTSIDE_H
