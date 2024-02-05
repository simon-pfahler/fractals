#include "julia.h"
#include "fractal_creation.h"

Julia::Julia(QWidget *parent)
    : QGraphicsView(parent), mCenter(0), mZ0(1i), mDeltareal(4.0), mIterations(30), mExponent(2)
{
    //deactivate scroll bars
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //set minimum size of the image
    this->setMinimumSize(500, 500);

    //initialize the scene and draw the fractal onto it
    mScene = new QGraphicsScene;

    mScene->addPixmap(frac_creator(this));
    this->setScene(mScene);
}

//setters & getters

std::complex<double> &Julia::center()
{
    return mCenter;
}

std::complex<double> const &Julia::center() const
{
    return mCenter;
}

std::complex<double> &Julia::z0()
{
    return mZ0;
}
std::complex<double> const &Julia::z0() const
{
    return mZ0;
}

double &Julia::deltareal()
{
    return mDeltareal;
}

double const &Julia::deltareal() const
{
    return mDeltareal;
}

unsigned int &Julia::iterations()
{
    return mIterations;
}

unsigned int const &Julia::iterations() const
{
    return mIterations;
}

double &Julia::exponent()
{
    return mExponent;
}

double const &Julia::exponent() const
{
    return mExponent;
}


//render the new fractal
void Julia::render(bool smooth)
{
    mScene->clear();
    if(smooth)
    {
        mScene->addPixmap(smooth_frac_creator(this));
    }
    else
    {
        mScene->addPixmap(frac_creator(this));
    }
    this->setScene(mScene);

    return;
}

void Julia::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_Plus: //zoom in to the set
            mDeltareal /= 1.5;
            render();
            break;
        case Qt::Key_Minus: //zoom out of the set
            mDeltareal *= 1.5;
            render();
            break;
        case Qt::Key_Right: //move to the right
            mCenter += mDeltareal/20;
            render();
            break;
        case Qt::Key_Left: //move to the left
            mCenter -= mDeltareal/20;
            render();
            break;
        case Qt::Key_Up: //move up
            mCenter += static_cast<std::complex<double>>(1i*mDeltareal/20);
            render();
            break;
        case Qt::Key_Down: //move down
            mCenter -= static_cast<std::complex<double>>(1i*mDeltareal/20);
            render();
            break;
        default:
            QGraphicsView::keyPressEvent(event);
            break;
    }
    return;
}
