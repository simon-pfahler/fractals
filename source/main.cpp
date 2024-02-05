#include "fractal_renderer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Fractal_Renderer w;
    w.show();
    return a.exec();
}
