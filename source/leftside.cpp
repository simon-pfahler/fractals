#include "leftside.h"

LeftSide::LeftSide(QWidget *parent) : QWidget(parent)
{
    //init left image
    mImage = new Mandelbrot(this);

    //init left iterator label
    mIterator = new QLabel("z<sub>n+1</sub>=z<sub>n</sub><sup>2</sup> + z<sub>0</sub>", this); //HTML formatted string
    mIterator->setAlignment(Qt::AlignCenter);

    //init left save button
    mSave = new QPushButton("Speichern", this);
    connect(mSave, &QPushButton::clicked, this, &LeftSide::save_image);
    mSave->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    //init a checkbox for wether to save the cross with the picture or not
    mSaveCross = new QCheckBox("save with cross", this);
    QFont f = mSaveCross->font();
    f.setPixelSize(12);
    mSaveCross->setFont(f);
    mSaveCross->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    mSaveCross->setCheckState(Qt::Unchecked);

    //put the button and the checkbox into a grid next to each other
    QGridLayout *saveLayout = new QGridLayout();
    saveLayout->addWidget(mSave, 0, 0);
    saveLayout->addWidget(mSaveCross, 0, 1);

    //add everything to the left layout
    QGridLayout *leftLayout = new QGridLayout();
    leftLayout->addWidget(mImage, 0, 0);
    leftLayout->addLayout(saveLayout, 1, 0);
    leftLayout->addWidget(mIterator, 2, 0);

    //set the layout of the widget as leftLayout
    this->setLayout(leftLayout);

    //connect signals and slots
    connect(mImage, &Mandelbrot::update_z0, this, &LeftSide::update_z0);
    connect(mImage, &Mandelbrot::progress_update, this, &LeftSide::progress_update);
}

void LeftSide::save_image()
{

    QString saveFile = QFileDialog::getSaveFileName(this, "Linkes Bild speichern", QString(), "Portable Network Graphics (*.png)");

    if(mSaveCross->checkState() == Qt::Unchecked) {
        mImage->remove_cross();
    }

    const QPixmap pixmap = mImage->grab();

    pixmap.save(saveFile);

    if(mSaveCross->checkState() == Qt::Unchecked) {
        mImage->add_cross();
    }

    return;
}

void LeftSide::update_iterations(unsigned int const &newIterations)
{
    //update the iterations number and render
    mImage->iterations()=newIterations;
    mImage->render();

    return;
}

void LeftSide::update_exponent(double const &newExponent)
{
    //update the exponent and render
    mImage->exponent()=newExponent;

    //update the label holding the iterator
    mIterator->setText("z<sub>n+1</sub>=z<sub>n</sub><sup>" + QString::number(newExponent) + "</sup> + z<sub>0</sub>");

    //reset the zoom when the exponent is changed (render is in here)
    reset_zoom();

    return;
}

void LeftSide::update_size()
{
    mImage->render();
    return;
}

void LeftSide::reset_zoom()
{
    mImage->center()= (mImage->exponent()==2) ? -0.5 : 0;
    mImage->deltareal()=4.0;
    mImage->render();
    return;
}

void LeftSide::render_smooth()
{
    mImage->render(true);
    return;
}
