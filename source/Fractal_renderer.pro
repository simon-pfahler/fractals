QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    fractal_creation.cpp \
    julia.cpp \
    leftside.cpp \
    main.cpp \
    fractal_renderer.cpp \
    mandelbrot.cpp \
    menu.cpp \
    rightside.cpp

HEADERS += \
    fractal_creation.h \
    fractal_renderer.h \
    julia.h \
    leftside.h \
    mandelbrot.h \
    menu.h \
    rightside.h

RC_ICONS = icon.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
