#include "rightside.h"

RightSide::RightSide(QWidget *parent) : QWidget(parent)
{
    //init right image
    mImage = new Julia(this);

    mIterator = new QLabel(getIterationLabel(), this); //HTML formatted string
    mIterator->setAlignment(Qt::AlignCenter);

    //init right save button
    mSave = new QPushButton("Speichern", this);
    connect(mSave, &QPushButton::clicked, this, &RightSide::save_image);

    QGridLayout *rightLayout = new QGridLayout();

    //add everything to the layout
    rightLayout->addWidget(mImage, 0, 2);
    rightLayout->addWidget(mSave, 1, 2);
    rightLayout->addWidget(mIterator, 2, 2);

    this->setLayout(rightLayout);

    //connect signals and slots
    connect(mImage, &Julia::progress_update, this, &RightSide::progress_update);
}

void RightSide::update_z0(std::complex<double> const &newZ0)
{
    //update the point and rerender
    mImage->z0() = newZ0;
    mImage->render();

    //additionally, change the label to hold the correct iterator
    mIterator->setText(getIterationLabel());

    return;
}

void RightSide::save_image() {

    QString saveFile = QFileDialog::getSaveFileName(this, "Rechtes Bild speichern", QString(), "Portable Network Graphics (*.png)");

    const QPixmap pixmap = mImage->grab();

    pixmap.save(saveFile);

    return;
}

void RightSide::update_iterations(unsigned int const &newIterations)
{
    //update the iterations number and render
    mImage->iterations()=newIterations;
    mImage->render();

    return;
}

void RightSide::update_exponent(double const &newExponent)
{
    //update the iterations number and render
    mImage->exponent()=newExponent;

    //update the label holding the iterator
    mIterator->setText(getIterationLabel());

    //reset the zoom when the exponent is changed (render is in here)
    reset_zoom();

    return;
}

void RightSide::update_size()
{
    mImage->render();
    return;
}

void RightSide::reset_zoom()
{
    mImage->center()=0;
    mImage->deltareal()=4.0;
    mImage->render();
    return;
}

void RightSide::render_smooth()
{
    mImage->render(true);
    return;
}

QString RightSide::getIterationLabel() {

    QString result = "z<sub>n+1</sub>=z<sub>n</sub><sup>";
    result += QString::number(mImage->exponent());
    result += "</sup> ";
    if(mImage->z0().real()>0)
    {
        result += "+ " + QString::number(mImage->z0().real());
    }
    else if(mImage->z0().real()<0) {
        result += "- " + QString::number(-mImage->z0().real());
    }
    if(mImage->z0().imag()>0)
    {
        result += " + " + ( (mImage->z0().imag()==1) ? "" : QString::number(mImage->z0().imag()) ) + "i";
    }
    else if(mImage->z0().imag()<0) {
        result += " - " + ( (mImage->z0().imag()==-1) ? "" : QString::number(-mImage->z0().imag()) ) + "i";
    }

    return result;
}
