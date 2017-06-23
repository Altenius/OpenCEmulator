#ifndef OPENCEMULATOR_SCREENSWIDGET_H
#define OPENCEMULATOR_SCREENSWIDGET_H


#include <QWidget>
#include <QPaintEvent>
#include <memory>

class Component;
typedef std::shared_ptr<Component> ComponentPtr;
typedef std::weak_ptr<Component> ComponentWeakPtr;

class ScreensWidget : public QWidget {
    Q_OBJECT
public:
    ScreensWidget(QWidget *parent = 0);
    
    inline void setScreen(const ComponentPtr &screen) {
        m_screen = screen;
    }
    
    inline void setKeyboard(const ComponentPtr &keyboard) {
        m_keyboard = keyboard;
    }
    
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

protected:
    void paintEvent(QPaintEvent *event) override;
    
private:
    ComponentWeakPtr m_screen;
    ComponentWeakPtr m_keyboard;
    
    int translateKey(int key);
    ushort translateChar(ushort c);
};

typedef std::weak_ptr<ScreensWidget> ScreensWidgetWeakPtr;
typedef std::shared_ptr<ScreensWidget> ScreensWidgetPtr;


#endif //OPENCEMULATOR_SCREENSWIDGET_H
