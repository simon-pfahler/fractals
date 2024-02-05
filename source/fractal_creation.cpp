#include "fractal_creation.h"

//coloring for points inside the set
constexpr QRgb inside_the_set = qRgb(70, 50, 80);

//coloring depending on the number of iterations a point needs to diverge
auto coloring = [](unsigned int iterations)
{
    int red = 255 * ( 0.5 + 0.5*sin(iterations/30.0*2*M_PI) );
    int green = 255 * ( 0.5 + 0.5*cos(iterations/30.0*2*M_PI) );
    int blue = 255 * ( 0.5 - 0.5*sin(iterations/30.0*2*M_PI) );
    return qRgb(red, green, blue);
};

//coloring depending on the distance from the set (used for smooth_frac_creator(Mandelbrot) )
auto smooth_coloring_Mandelbrot = [](unsigned int distance)
{
    int red = 255 * ( 0.5 + 0.5*sin(log(distance)/5.7*2*M_PI) );
    int green = 255 * ( 0.5 + 0.5*cos(log(distance)/5.7*2*M_PI) );
    int blue = 255 * ( 0.5 - 0.5*sin(log(distance)/5.7*2*M_PI) );
    return qRgb(red, green, blue);
};

//coloring depending on the distance from the set (used for smooth_frac_creator(Julia) )
auto smooth_coloring_Julia = [](unsigned int distance)
{
    int red = 255 * ( 0.5 + 0.5*sin(M_PI + log(distance)/5.7*2*M_PI) );
    int green = 255 * ( 0.5 + 0.5*cos(M_PI + log(distance)/5.7*2*M_PI) );
    int blue = 255 * ( 0.5 - 0.5*sin(M_PI + log(distance)/5.7*2*M_PI) );
    return qRgb(red, green, blue);
};

QPixmap frac_creator (Mandelbrot *const &object)
{

    //send signal to the progress bar telling rendering is starting
    emit object->progress_update( 0 );

    QImage *frac = new QImage(object->width(), object->height(), QImage::Format_RGB32);

    //color for points inside the set
    frac->fill(inside_the_set);

    //loop all pixels
    for(int w = 0; w < object->width(); ++w)
    {
        for(int h = 0; h < object->height(); ++h)
        {

            std::complex<double> z=pixels_to_complex(w, h, object);
            std::complex<double> z0 = z;

            //check for cardioid and period-2 bulb to get faster rendering (only if exponent=2)
            if(object->exponent() == 2)
            {
                //check for the period-2 bulb
                if( (z0.real()+1)*(z0.real()+1) + z0.imag()*z0.imag() <=1/16.0 )
                {
                    //just go to the next pixel, as the inside color is already set by default
                    continue;
                }

                double help = (z0.real()-0.25)*(z0.real()-0.25) + z0.imag()*z0.imag();

                //check for the cardioid
                if( help*(help+(z0.real()-0.25)) <= 0.25*z0.imag()*z0.imag() )
                {
                    //just go to the next pixel, as the inside color is already set by default
                    continue;
                }
            }

            //evaluate when/if this point leaves the set
            for(unsigned int i = 0; i<object->iterations(); ++i)
            {
                z = pow( z, object->exponent() ) + z0;
                if(std::norm(z)>4) {

                    //color the pixel accordingly, then go to the next pixel
                    frac->setPixel(w, h, coloring(i));
                    break;
                }
            }
        }

        //send signal to the progress bar telling the current progress
        emit object->progress_update( static_cast<double>(100*w) /object->width() );
    }

    //send signal to the progress bar telling rendering is done
    emit object->progress_update( 100 );

    return QPixmap::fromImage(*frac);
}

QPixmap smooth_frac_creator(Mandelbrot *const &object)
{
    //first render the usual Mandelbrot set
    QImage *frac = new QImage(object->width(), object->height(), QImage::Format_RGB32);
    *frac = frac_creator(object).toImage();

    //send signal to the progress bar telling coloring step of rendering is starting
    emit object->progress_update( 0 );

    //loop all pixels
    for(int w = 0; w < object->width(); ++w)
    {
        //always holds the last distance calculated, reset when going to the next column
        int d = 0;

        for(int h = 0; h < object->height(); ++h)
        {
            //only change the coloring if the point is outside the set
            if(frac->pixel(w, h) != inside_the_set)
            {
                //get the distance from the set (the distance has to be at least one less than previous distance, since we only went 1 pixel)
                d = get_distance(w, h, frac, std::max(d-1, 0));

                //color the point accordingly
                frac->setPixel(w, h, smooth_coloring_Mandelbrot(d));
            }
        }

        //send signal to the progress bar telling the current progress
        emit object->progress_update( static_cast<double>(100*w) /object->width() );
    }

    //send signal to the progress bar telling rendering is done
    emit object->progress_update( 100 );

    return QPixmap::fromImage(*frac);
}

QPixmap frac_creator (Julia *const &object)
{

    //send signal to the progress bar telling rendering is starting
    emit object->progress_update( 100 );

    QImage *frac = new QImage(object->width(), object->height(), QImage::Format_RGB32);

    //color for points inside the set
    frac->fill(inside_the_set);

    //loop all pixels
    for(int w = 0; w < object->width(); ++w)
    {
        for(int h = 0; h < object->height(); ++h)
        {
            std::complex<double> z=pixels_to_complex(w, h, object);

            //evaluate when/if this point leaves the set
            for(unsigned int i = 0; i<object->iterations(); ++i)
            {
                z = pow( z, object->exponent() ) + object->z0();
                if(std::norm(z)>4) {

                    //color the pixel accordingly, then go to the next pixel
                    frac->setPixel(w, h, coloring(i));
                    break;
                }
            }

        }

        //send signal to the progress bar telling the current progress
        emit object->progress_update( static_cast<double>(100*w) /object->width() );
    }

    //send signal to the progress bar telling rendering is done
    emit object->progress_update( 100 );

    return QPixmap::fromImage(*frac);
}

QPixmap smooth_frac_creator(Julia *const &object)
{
    //first render the usual Mandelbrot set
    QImage *frac = new QImage(object->width(), object->height(), QImage::Format_RGB32);
    *frac = frac_creator(object).toImage();

    //send signal to the progress bar telling coloring step of rendering is starting
    emit object->progress_update( 0 );

    //loop all pixels
    for(int w = 0; w < object->width(); ++w)
    {
        //always holds the last distance calculated, reset when going to the next column
        int d = 0;

        for(int h = 0; h < object->height(); ++h)
        {
            //only change the coloring if the point is outside the set
            if(frac->pixel(w, h) != inside_the_set)
            {
                //get the distance from the set (the distance has to be at least one less than previous distance, since we only went 1 pixel)
                d = get_distance(w, h, frac, std::max(d-1, 0));

                //color the point accordingly
                frac->setPixel(w, h, smooth_coloring_Julia(d));
            }
        }

        //send signal to the progress bar telling the current progress
        emit object->progress_update( static_cast<double>(100*w) /object->width() );
    }

    //send signal to the progress bar telling rendering is done
    emit object->progress_update( 100 );

    return QPixmap::fromImage(*frac);
}

unsigned int get_distance(unsigned int const &w, unsigned int const &h, QImage *const &frac, unsigned int const &minRadius )
{
    //lambda expression to check if a pixel is in the fractal
    auto check_one = [&](unsigned int posx, unsigned int posy)
    {
        //we don't have to check for posx<=0 because negative values will underflow to positive values way above frac->width() and frac->height()
        if( posx < static_cast<unsigned int>(frac->width()) && posy<static_cast < unsigned int>(frac->height()) )
        {
            if(frac->pixel(posx, posy) == inside_the_set) { return true; }
        }
        return false;
    };


    //lambda function to check if a pixel or one of its four direct neighbours is int the fractal
    auto check = [&](unsigned int posx, unsigned int posy)
    {
        if(check_one(posx, posy))   { return true; } //check the point itself
        if(check_one(posx+1, posy)) { return true; } //check
        if(check_one(posx-1, posy)) { return true; } //its
        if(check_one(posx, posy+1)) { return true; } //four
        if(check_one(posx, posy-1)) { return true; } //neighbours
        return false;
    };

    //check all distances starting with minRadius
    for(unsigned int radius = minRadius; radius < static_cast<unsigned int>(std::max(frac->width(), frac->height())); ++radius)
    {

        for(unsigned int x = 0; x*x < 0.5*radius*radius; ++x)
        {
            unsigned int y = sqrt(radius*radius-x*x);

            //check the corresponding point in every quadrant
            if(check(w+x, h+y)) { return radius; }
            if(check(w-x, h+y)) { return radius; }
            if(check(w+x, h-y)) { return radius; }
            if(check(w-x, h-y)) { return radius; }

            //also check the points mirrored along the diagonal
            if(check(w+y, h+x)) { return radius; }
            if(check(w-y, h+x)) { return radius; }
            if(check(w+y, h-x)) { return radius; }
            if(check(w-y, h-x)) { return radius; }
        }
    }

    //if no point in th image is in the set, return the maximum possible value
    return static_cast<unsigned int>(std::max(frac->width(), frac->height()));
}
