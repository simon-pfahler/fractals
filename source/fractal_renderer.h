#ifndef FRACTAL_RENDERER_H
#define FRACTAL_RENDERER_H

#include <QWidget>
#include <QLabel>
#include <QResizeEvent>
#include <QTimer>

#include <complex>

#include "leftside.h"
#include "rightside.h"
#include "menu.h"

class Fractal_Renderer : public QWidget
{
    Q_OBJECT

public:
    Fractal_Renderer(QWidget *parent = nullptr);
    ~Fractal_Renderer();
    void resizeEvent(QResizeEvent *event);

private:

    //QWidgets for the left and right side and the menu
    LeftSide *mLeft;
    RightSide *mRight;
    Menu *mMenu;

    //QTimer that triggers a rerender when time is up
    QTimer *mTimer;

};
#endif // FRACTAL_RENDERER_H
