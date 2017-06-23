#include "ComponentKeyboard.h"
#include "ComponentScreen.h"
#include "Instance.h"



ComponentKeyboard::ComponentKeyboard(ComponentPtr screen, const std::string &uuid, const std::string &label)
        : Component("keyboard", uuid, label), m_screen(screen)
{

}



void ComponentKeyboard::keyPressed(int code, int key)
{
    ArgList args;
    args.add(m_uuid);
    args.add(static_cast<long long>(code));
    args.add(static_cast<long long>(key));
    args.add("Player");

    for (InstanceWeakPtr i : m_instances) {
        if (auto instance = i.lock()) {
            instance->signal("key_down", args);
        }
    }
}



void ComponentKeyboard::keyReleased(int code, int key)
{
    ArgList args;
    args.add(m_uuid);
    args.add(static_cast<long long>(code));
    args.add(static_cast<long long>(key));
    args.add("Player");

    for (InstanceWeakPtr i : m_instances) {
        if (auto instance = i.lock()) {
            instance->signal("key_up", args);
        }
    }
}



void ComponentKeyboard::cleanup()
{
    if (auto screen = m_screen.lock()) {
        std::static_pointer_cast<ComponentScreen>(screen)->detachKeyboard(this);
    }
    Component::cleanup();
}



void ComponentKeyboard::setScreen(const ComponentPtr &screen)
{
    if (auto screen = m_screen.lock()) {
        std::static_pointer_cast<ComponentScreen>(screen)->detachKeyboard(this);
    }
    m_screen = screen;
}



ComponentKeyboard::~ComponentKeyboard()
{

}
