#include <OpenCEmulator.h>
#include <iostream>
#include "KeyboardModifier.h"
#include "components/ComponentScreen.h"
#include "ui_keyboardmodifier.h"



KeyboardModifier::KeyboardModifier(QWidget *parent) : ComponentModifier(parent), m_ui(new Ui::KeyboardModifier)
{
    m_ui->setupUi(this);

    connect(&OpenCEmulator::get(), &OpenCEmulator::componentsChanged, this, &KeyboardModifier::componentsChanged);
}



Q_DECLARE_METATYPE(ComponentWeakPtr);



void KeyboardModifier::componentsChanged(const std::vector<ComponentPtr> &components)
{
    m_ui->comboScreen->clear();
    for (const ComponentPtr &component : components) {
        if (component->type() == "screen") {
            m_ui->comboScreen->addItem(QString::fromStdString(component->label()),
                                       QVariant::fromValue(ComponentWeakPtr(component)));
        }
    }
}



void KeyboardModifier::update(const ComponentPtr &component)
{
    ComponentModifier::update(component);
    assert(component->type() == "keyboard");

    const ComponentKeyboard *keyboard = static_cast<ComponentKeyboard *>(component.get());
    auto screen = keyboard->screen();
    if (screen) {
        m_ui->comboScreen->setCurrentIndex(m_ui->comboScreen->findText(QString::fromStdString(screen->label())));
    }
}



void KeyboardModifier::on_comboScreen_currentIndexChanged(int index)
{
    if (auto component = m_component.lock()) {
        auto keyboard = std::static_pointer_cast<ComponentKeyboard>(component);
        ComponentPtr componentScreen = m_ui->comboScreen->itemData(index).value<ComponentWeakPtr>().lock();

        if (componentScreen) {
            auto screen = std::static_pointer_cast<ComponentScreen>(componentScreen);
            if (auto oldScreen = keyboard->screen()) {
                std::static_pointer_cast<ComponentScreen>(oldScreen)->detachKeyboard(component);
            }

            keyboard->setScreen(componentScreen);
            screen->attachKeyboard(component);

            const ScreensWidgetPtr &widget = screen->widget();
            if (widget) {
                widget->setKeyboard(component);
            }
        }
    }
}



ComponentPtr KeyboardModifier::create(const std::string &label)
{
    ComponentPtr screen = OpenCEmulator::get().findComponent(m_ui->comboScreen->currentText().toStdString());
    if (screen) {
        return ComponentPtr(new ComponentKeyboard(screen, std::string(), label));
    }

    return ComponentPtr(new ComponentKeyboard(nullptr, std::string(), label));
}
