#ifndef OPENCEMULATOR_COMPONENTKEYBOARD_H
#define OPENCEMULATOR_COMPONENTKEYBOARD_H


#include "Component.h"

class ComponentScreen;

class ComponentKeyboard : public Component
{
public:
    ComponentKeyboard(ComponentPtr screen = nullptr, const std::string &uuid = std::string(),
                      const std::string &label = std::string());

    ~ComponentKeyboard();

    void keyPressed(int code, int key);

    void keyReleased(int code, int key);

    void cleanup() override;



    inline std::shared_ptr<Component> screen() const
    {
        return m_screen.lock();
    }



    void setScreen(const ComponentPtr &screen);

private:
    ComponentWeakPtr m_screen;
};


#endif //OPENCEMULATOR_COMPONENTKEYBOARD_H
