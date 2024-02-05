#include "mandelbrot.h"
#include "fractal_creation.h"

Mandelbrot::Mandelbrot(QWidget *parent)
    : QGraphicsView(parent), mCenter(-0.5), mDeltareal(4.0), mIterations(30), mExponent(2), mCrossNumber(1i)
{
    //deactivate scroll bars
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //set minimum size of the image
    this->setMinimumSize(500, 500);

    //init the Scene
    mScene = new QGraphicsScene;

    //init the lines
    mLine1 = new QGraphicsLineItem;
    mLine2 = new QGraphicsLineItem;

    //add the fractal to the scene (the size is correct because frac_creator uses this->width() and this->height() to get the right size
    mScene->addPixmap(frac_creator(this));

    //get the cross position
    unsigned int posx = complex_to_posx(mCrossNumber, this);
    unsigned int posy = complex_to_posy(mCrossNumber, this);

    //get the color of the pixel the cross is centered on and derive the cross color from it
    QColor newColor = this->grab().toImage().pixelColor(posx, posy);
    newColor.setRgb(255-127*pow(newColor.red()/255.0, 2), 255-127*pow(newColor.green()/255.0, 2), 255-127*pow(newColor.blue()/255.0, 2));

    //set an individual pen for the lines of the cross
    QPen pen = QPen();
    pen.setWidth(4);
    pen.setCapStyle(Qt::RoundCap);
    pen.setColor(newColor);

    //initialize the lines
    mLine1->setPen(pen);
    mLine2->setPen(pen);
    mLine1->setLine(posx-5, posy-5, posx+5, posy+5);
    mLine2->setLine(posx-5, posy+5, posx+5, posy-5);

    //add the lines to the Scene
    mScene->addItem(mLine1);
    mScene->addItem(mLine2);

    //draw everything to the scene
    this->setScene(mScene);
}

//setters & getters
std::complex<double> &Mandelbrot::center()
{
    return mCenter;
}

std::complex<double> const &Mandelbrot::center() const
{
    return mCenter;
}

double &Mandelbrot::deltareal()
{
    return mDeltareal;
}

double const &Mandelbrot::deltareal() const
{
    return mDeltareal;
}

unsigned int &Mandelbrot::iterations()
{
    return mIterations;
}

unsigned int const &Mandelbrot::iterations() const
{
    return mIterations;
}

double &Mandelbrot::exponent()
{
    return mExponent;
}

double const &Mandelbrot::exponent() const
{
    return mExponent;
}

//render the new fractal, number is the number corresponding to the current Julia set
void Mandelbrot::render(bool smooth)
{

    //remove the lines from the scene so they don't get deleted by mScene->clear()
    mScene->removeItem(mLine1);
    mScene->removeItem(mLine2);

    //clear the scene and add the new fractal (fractal_creator implicitly gets the correct size)
    mScene->clear();
    if(smooth)
    {
        mScene->addPixmap(smooth_frac_creator(this));
    }
    else
    {
        mScene->addPixmap(frac_creator(this));
    }

    //get the new cross position
    unsigned int posx = complex_to_posx(mCrossNumber, this);
    unsigned int posy = complex_to_posy(mCrossNumber, this);

    //two Lines overlapping appear as a cross
    mLine1->setLine(posx-6, posy-6, posx+6, posy+6);
    mLine2->setLine(posx-6, posy+6, posx+6, posy-6);

    //add the lines to the Scene again and implicitly set their parent to mScene again
    mScene->addItem(mLine1);
    mScene->addItem(mLine2);

    this->setScene(mScene);

    return;
}


void Mandelbrot::mousePressEvent(QMouseEvent *event)
{
    //update the position of the cross, update_cross() sends a signal that the Julia set should be rerendered
    mCrossNumber = pixels_to_complex(event->x(), event->y(), this);
    update_cross();

    return;
}

void Mandelbrot::keyPressEvent(QKeyEvent *event)
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
        case Qt::Key_W: //move the cross up
            mCrossNumber += static_cast<std::complex<double>>(1i*mDeltareal/100);
            update_cross();
            break;
        case Qt::Key_A: //move the cross to the left
            mCrossNumber -= mDeltareal/100;
            update_cross();
            break;
        case Qt::Key_S: //move the cross down
            mCrossNumber -= static_cast<std::complex<double>>(1i*mDeltareal/100);
            update_cross();
            break;
        case Qt::Key_D: //move the cross to the right
            mCrossNumber += mDeltareal/100;
            update_cross();
            break;
        default:
            QGraphicsView::keyPressEvent(event);
            break;
    }
    return;
}

void Mandelbrot::remove_cross()
{
    mScene->removeItem(mLine1);
    mScene->removeItem(mLine2);

    return;
}

void Mandelbrot::add_cross()
{
    mScene->addItem(mLine1);
    mScene->addItem(mLine2);

    return;
}

void Mandelbrot::update_cross()
{
    //don't draw the cross too close to the border, that makes it behave weird
    unsigned int posx = std::min( std::max( complex_to_posx(mCrossNumber, this), 6U ), static_cast<unsigned int>(this->width()-6) );
    unsigned int posy = std::min (std::max( complex_to_posy(mCrossNumber, this), 6U ), static_cast<unsigned int>(this->height()-6) );

    //get the color of the pixel the cross is centered on and derive the cross color from it (this is trying to draw the cross in a color distinctive from the background)
    QColor newColor = this->grab().toImage().pixelColor(posx, posy);
    newColor.setRgb(255-127*pow(newColor.red()/255.0, 2), 255-127*pow(newColor.green()/255.0, 2), 255-127*pow(newColor.blue()/255.0, 2));

    //set the cross color
    QPen pen = QPen(Qt::red, 4, Qt::SolidLine, Qt::RoundCap);
    pen.setWidth(4);
    pen.setCapStyle(Qt::RoundCap);
    pen.setColor(newColor);
    mLine1->setPen(pen);
    mLine2->setPen(pen);

    //set the Lines to the new position
    mLine1->setLine(posx-6, posy-6, posx+6, posy+6);
    mLine2->setLine(posx-6, posy+6, posx+6, posy-6);

    //rerender the Julia set for the new point
    emit update_z0(pixels_to_complex(posx, posy, this));

    return;
}
