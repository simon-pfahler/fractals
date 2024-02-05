#include "fractal_renderer.h"
#include "fractal_creation.h"

Fractal_Renderer::Fractal_Renderer(QWidget *parent)
    : QWidget(parent)
{
    //set the title of the window
    this->setWindowTitle("Fraktale Renderer");

    //set the default font to a bigger font
    QFont f = this->font();
    f.setPixelSize(30);
    this->setFont(f);

    //init Menu Label
    QLabel *mMenuTitle = new QLabel("<b><u>Menu</u></b>", this);
    mMenuTitle->setAlignment(Qt::AlignCenter);

    //init left title
    QLabel *mLeftTitle = new QLabel("<b><u>Mandelbrot-Menge</u></b>", this);
    mLeftTitle->setAlignment(Qt::AlignCenter);

    //init right title
    QLabel *mRightTitle = new QLabel("<b><u>Julia-Menge</u></b>", this);
    mRightTitle->setAlignment(Qt::AlignCenter);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setColumnMinimumWidth(1, 250);

    //init the three sections
    mRight = new RightSide;
    mMenu = new Menu;
    mLeft = new LeftSide;

    //set the layout of the Window
    mainLayout->addWidget(mLeftTitle, 0, 0);
    mainLayout->addWidget(mMenuTitle, 0, 1);
    mainLayout->addWidget(mRightTitle, 0, 2);
    mainLayout->addWidget(mLeft, 1, 0);
    mainLayout->addWidget(mMenu, 1, 1);
    mainLayout->addWidget(mRight, 1, 2);

    this->setLayout(mainLayout);

    //init the QTimer
    mTimer = new QTimer(this);

    //connect the signals to the slots
    connect(mLeft, &LeftSide::update_z0, mRight, &RightSide::update_z0);

    connect(mRight, &RightSide::progress_update, mMenu, &Menu::progress_update);
    connect(mLeft, &LeftSide::progress_update, mMenu, &Menu::progress_update);

    connect(mMenu, &Menu::update_iterations, mRight, &RightSide::update_iterations);
    connect(mMenu, &Menu::update_iterations, mLeft, &LeftSide::update_iterations);

    connect(mMenu, &Menu::update_exponent, mRight, &RightSide::update_exponent);
    connect(mMenu, &Menu::update_exponent, mLeft, &LeftSide::update_exponent);

    connect(mMenu, &Menu::reset_zoom, mRight, &RightSide::reset_zoom);
    connect(mMenu, &Menu::reset_zoom, mLeft, &LeftSide::reset_zoom);

    connect(mMenu, &Menu::render_smooth, mRight, &RightSide::render_smooth);
    connect(mMenu, &Menu::render_smooth, mLeft, &LeftSide::render_smooth);

    connect(mTimer, &QTimer::timeout, mRight, &RightSide::update_size);
    connect(mTimer, &QTimer::timeout, mLeft, &LeftSide::update_size);
    connect(mTimer, &QTimer::timeout, mTimer, &QTimer::stop);
}

Fractal_Renderer::~Fractal_Renderer()
{
}

void Fractal_Renderer::resizeEvent(QResizeEvent *event)
{
    //starts a timer, so this timer runs out 100ms after the last resizeEvent, its Signal is then used to update the size of the fractals
    mTimer->start(100);
    return;
}
