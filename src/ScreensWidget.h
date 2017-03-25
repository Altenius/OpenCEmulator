#ifndef OPENCEMULATOR_SCREENSWIDGET_H
#define OPENCEMULATOR_SCREENSWIDGET_H


#include <QWidget>
#include <QPaintEvent>

class ScreensWidget : public QWidget {
    Q_OBJECT
public:
    ScreensWidget(QWidget *parent = 0);
    
    

protected:
    void paintEvent(QPaintEvent *event) override;
};


#endif //OPENCEMULATOR_SCREENSWIDGET_H
