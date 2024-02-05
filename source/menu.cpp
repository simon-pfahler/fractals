#include "menu.h"

Menu::Menu(QWidget *parent) : QWidget(parent)
{
    //init the Iteration section
    mIterationsLabel = new QLabel("Iterationen: 30", this);
    mIterationsLabel->setAlignment(Qt::AlignHCenter);

    mIterationsSlider = new QSlider(Qt::Horizontal, this);
    mIterationsSlider->setMinimum(1);
    mIterationsSlider->setMaximum(50);
    mIterationsSlider->setValue(3);
    connect(mIterationsSlider, &QAbstractSlider::sliderReleased, this, &Menu::call_iter_update);

    QLabel *mIterationsSliderLowerBound = new QLabel("10", this);
    mIterationsSliderLowerBound->setAlignment(Qt::AlignHCenter);
    QLabel *mIterationsSliderUpperBound = new QLabel("500", this);
    mIterationsSliderUpperBound->setAlignment(Qt::AlignHCenter);

    //set the iteration section up in a GridLayout
    QGridLayout *mIterationLayout = new QGridLayout;

    mIterationLayout->addWidget(mIterationsLabel, 0, 0, 1, 3);
    mIterationLayout->addWidget(mIterationsSliderLowerBound, 1, 0);
    mIterationLayout->addWidget(mIterationsSlider, 1, 1);
    mIterationLayout->addWidget(mIterationsSliderUpperBound, 1, 2);


    //init the Exponent section
    mExponentLabel = new QLabel("Exponent: 2", this);
    mExponentLabel->setAlignment(Qt::AlignHCenter);

    mExponentSlider = new QSlider(Qt::Horizontal, this);
    mExponentSlider->setMinimum(0);
    mExponentSlider->setMaximum(5);
    mExponentSlider->setValue(2);
    connect(mExponentSlider, &QAbstractSlider::sliderReleased, this, &Menu::exp_changed);

    QLabel *mExponentSliderLowerBound = new QLabel("0", this);
    mIterationsSliderLowerBound->setAlignment(Qt::AlignHCenter);
    QLabel *mExponentSliderUpperBound = new QLabel("5", this);
    mIterationsSliderUpperBound->setAlignment(Qt::AlignHCenter);

    QFont f =this->font();
    f.setPixelSize(12);

    mOnlyIntExps = new QCheckBox("nur ganzzahlige Exponenten", this);
    mOnlyIntExps->setFont(f);
    mOnlyIntExps->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    mOnlyIntExps->setCheckState(Qt::Checked);
    connect(mOnlyIntExps, &QCheckBox::stateChanged, this, &Menu::change_exp_allowed_values);

    //set the Exponent section up in a GridLayout
    QGridLayout *mExponentLayout = new QGridLayout;

    mExponentLayout->addWidget(mExponentLabel, 0, 0, 1, 3);
    mExponentLayout->addWidget(mExponentSliderLowerBound, 1, 0);
    mExponentLayout->addWidget(mExponentSlider, 1, 1);
    mExponentLayout->addWidget(mExponentSliderUpperBound, 1, 2);
    mExponentLayout->addWidget(mOnlyIntExps, 2, 1, 1, 2);


    //init the reset button
    mResetZoom = new QPushButton("Reset Zoom", this);
    connect(mResetZoom, &QPushButton::clicked, this, &Menu::reset_zoom);


    //init the smooth button
    mSmooth = new QPushButton("kontinuierlich färben", this);
    f.setPixelSize(20);
    mSmooth->setFont(f);
    connect(mSmooth, &QPushButton::clicked, this, &Menu::render_smooth);


    //init the progress bar
    mProgressBar = new QProgressBar(this);
    mProgressBar->setVisible(false);
    mProgressBar->setTextVisible(false);
    mProgressBar->setMinimum(0);
    mProgressBar->setMaximum(100);
    mProgressBar->setValue(0);


    //arrange the Menu in an own GridLayout
    QGridLayout *menuLayout = new QGridLayout;

    //QSpacerItem to align the menu correctly
    QSpacerItem *mSpaceAtBottom = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem *mSpaceBetweenSections = new QSpacerItem(0, 50, QSizePolicy::Minimum, QSizePolicy::Minimum);

    menuLayout->addLayout(mIterationLayout, 0, 0);
    menuLayout->addItem(mSpaceBetweenSections, 1, 0);
    menuLayout->addLayout(mExponentLayout, 2, 0);
    menuLayout->addItem(mSpaceBetweenSections, 3, 0);
    menuLayout->addWidget(mResetZoom, 4, 0);
    menuLayout->addItem(mSpaceBetweenSections, 5, 0);
    menuLayout->addWidget(mSmooth, 5, 0);
    menuLayout->addItem(mSpaceAtBottom, 7, 0);
    menuLayout->addWidget(mProgressBar, 8, 0);

    this->setLayout(menuLayout);


    //set maximum width of the menu widget
    this->setMaximumWidth(250);

}

void Menu::call_iter_update()
{
    //update QIterationsLabel to hold the correct number, then tell the fractals about it for a rerender
    mIterationsLabel->setText("Iterationen: " + QString::number(mIterationsSlider->value()*10));

    emit update_iterations(mIterationsSlider->value()*10);

    return;
}

void Menu::exp_changed()
{
    //check if the slider is in integer or double mode
    if(mOnlyIntExps->checkState() == Qt::Unchecked)
    {
        //in double mode, we have to divide the sliders value by 10

        //update QExponentLabel to hold the correct number, then tell the fractals about it for a rerender
        mExponentLabel->setText("Exponent: " + QString::number(mExponentSlider->value()/10.0));

        emit update_exponent(mExponentSlider->value()/10.0);
    }
    else
    {
        //in integer mode, the slider value is the same as the exponent

        //update QExponentLabel to hold the correct number, then tell the fractals about it for a rerender
        mExponentLabel->setText("Exponent: " + QString::number(mExponentSlider->value()));

        emit update_exponent(mExponentSlider->value());
    }

    return;
}

void Menu::change_exp_allowed_values(int const &newState)
{
    if(newState == Qt::Unchecked)
    {
        //set the new range to 50 and update the position to 10 x the previous position
        mExponentSlider->setMaximum(50);
        mExponentSlider->setValue(mExponentSlider->value()*10);

        //update QExponentLabel to hold the correct number
        mExponentLabel->setText("Exponent: " + QString::number(mExponentSlider->value()/10));

        //no need to rerender because the new slider position corresponds to the same value

    }
    else
    {

        //update the exponent if it is not an integer
        if(mExponentSlider->value()%10 != 0)
        {
            //update the exponent if it was changed
            emit update_exponent(round(mExponentSlider->value()/10.0));
        }

        //change the slider to the closest integer
        mExponentSlider->setValue(round(mExponentSlider->value()/10.0));

        //set new range of the slider
        mExponentSlider->setMaximum(5);

        //update QExponentLabel to hold the correct number
        mExponentLabel->setText("Exponent: " + QString::number(mExponentSlider->value()));
    }
    return;
}

void Menu::progress_update(unsigned int const &progress)
{

    if(progress == 0)
    {
        mProgressBar->setVisible(true);
    }

    mProgressBar->setValue(progress);

    if(static_cast<int>(progress) == mProgressBar->maximum())
    {
        mProgressBar->setVisible(false);
        mProgressBar->setValue(0);
        mProgressBar->setMaximum(100);
    }

    return;
}
