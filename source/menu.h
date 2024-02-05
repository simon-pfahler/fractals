#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QProgressBar>

#include <cmath>

class Menu : public QWidget
{
    Q_OBJECT
public:
    explicit Menu(QWidget *parent = nullptr);

signals:
    void update_iterations(unsigned int const &newIterations);
    void update_exponent(double const &newExponent);
    void reset_zoom();
    void render_smooth();

public slots:
    void progress_update(unsigned int const &progress);

private slots:
    void call_iter_update();
    void exp_changed();
    void change_exp_allowed_values(int const &newState);

private:

    //member data for the menu
    QLabel *mIterationsLabel; //Label with "Iterationen: #Wert"
    QSlider *mIterationsSlider; //slider to see how many iterations to do (slider->value() = 0.1 * iterations
    QPushButton *mResetZoom; //Button to reset the zoom of both sets
    QLabel *mExponentLabel; //Label with "Exponent: #Wert"
    QSlider *mExponentSlider; //slider to set the exponent in the iterator ( slider->value() = 10 * exponent )
    QCheckBox *mOnlyIntExps; //if checked, only integers can be selected as exponent
    QProgressBar *mProgressBar; //Progress Bar to show the current rendering progress
    QPushButton *mSmooth; //if clicked, renders the distance-based colorings of both fractals ONCE (it doesn't set this as default as it is very slow)
};

#endif // MENU_H
