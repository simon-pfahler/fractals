#ifndef FRACTAL_CREATION_H
#define FRACTAL_CREATION_H

#include <QImage>
#include <QPixmap>

#include <complex>
#include <cmath>

#include "mandelbrot.h"
#include "julia.h"

//returns the complex value represented by the particular pixel (T has to be Mandelbrot or Julia)
template <typename T>
std::complex<double> pixels_to_complex(unsigned int const &posx, unsigned int const &posy, T *const &object)
{
    double const deltaimag = static_cast<double>(object->height())/object->width() * object->deltareal();

    return object->center() - object->deltareal()/2 + (static_cast<double>(posx)/object->width())*object->deltareal() + //real part of the result
            static_cast< std::complex<double> >( 1i ) * (deltaimag/2 - (static_cast<double>(posy)/object->height())*deltaimag); //imaginary part of the result
}

//returns the pixel position in x direction of complex number, use with caution, not too precise (T has to be Mandelbrot or Julia)
template <typename T>
unsigned int complex_to_posx(std::complex<double> const &nr, T *const &object)
{
    return static_cast<unsigned int>( ( nr.real() - ( object->center().real()-object->deltareal()/2 ) ) * object->width()/object->deltareal());
}

//returns the pixel position in y direction of complex number, use with caution, not too precise (T has to be Mandelbrot or Julia)
template <typename T>
unsigned int complex_to_posy(std::complex<double> const &nr, T *const &object)
{
    double const deltaimag = static_cast<double>(object->height())/object->width() * object->deltareal();

    return static_cast<unsigned int>( ( - nr.imag() + ( object->center().imag()+deltaimag/2 ) ) * object->height()/deltaimag );
}

//returns the colored image containing the respective fractal
QPixmap frac_creator (Mandelbrot *const &object);
QPixmap smooth_frac_creator(Mandelbrot *const &object);
QPixmap frac_creator (Julia *const &object);
QPixmap smooth_frac_creator(Julia *const &object);

//returns the distance of the point (w,h) to the fractal in frac (if you know the distance is at least minRadius, please tell)
unsigned int get_distance(unsigned int const &w, unsigned int const &h, QImage *const &frac, unsigned int const &minRadius = 0);

#endif // FRACTAL_CREATION_H
