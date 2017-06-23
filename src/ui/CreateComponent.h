#ifndef OPENCEMULATOR_CREATECOMPONENT_H
#define OPENCEMULATOR_CREATECOMPONENT_H


#include <QtWidgets/QWidget>

namespace Ui {
    class CreateComponent;
}

class ComponentModifier;
class KeyboardModifier;

class CreateComponent : public QWidget {
    Q_OBJECT
public:
    explicit CreateComponent(QWidget *parent = 0);
    
private:
    Ui::CreateComponent *m_ui;
    
    ComponentModifier *m_modifier;
    KeyboardModifier *m_keyboardModifier;
    
private slots:
    void on_comboTypes_currentIndexChanged(const QString &text);
    void on_buttonCreate_clicked();
};


#endif //OPENCEMULATOR_CREATECOMPONENT_H
