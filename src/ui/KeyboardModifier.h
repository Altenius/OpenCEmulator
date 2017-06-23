#ifndef OPENCEMULATOR_KEYBOARDMODIFIER_H
#define OPENCEMULATOR_KEYBOARDMODIFIER_H


#include "ComponentModifier.h"

namespace Ui {
    class KeyboardModifier;
}

class KeyboardModifier : public ComponentModifier {
    Q_OBJECT;
public:
    explicit KeyboardModifier(QWidget *parent = 0);

    void update(const ComponentPtr &component) override;
    
    ComponentPtr create(const std::string &label) override;
    
private:
    Ui::KeyboardModifier *m_ui;
    
private slots:
    void componentsChanged(const std::vector<ComponentPtr> &components);
    void on_comboScreen_currentIndexChanged(int index);
};


#endif //OPENCEMULATOR_KEYBOARDMODIFIER_H
