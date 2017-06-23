#ifndef OPENCEMULATOR_COMPONENTMODIFIER_H
#define OPENCEMULATOR_COMPONENTMODIFIER_H


#include <components/Component.h>
#include <QWidget>

class ComponentModifier : public QWidget {
    Q_OBJECT;
public:
    explicit ComponentModifier(QWidget *parent = 0);
    
    virtual void apply(const ComponentPtr &component) {};
    
    virtual ComponentPtr create(const std::string &label) =0;
    
    virtual void update(const ComponentPtr &component) {
        m_component = component;
    };
    
protected:
    ComponentWeakPtr m_component;
};


#endif //OPENCEMULATOR_COMPONENTMODIFIER_H
