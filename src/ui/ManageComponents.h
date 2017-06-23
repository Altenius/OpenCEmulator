#ifndef OPENCEMULATOR_MANAGECOMPONENTS_H
#define OPENCEMULATOR_MANAGECOMPONENTS_H


#include <QWidget>
#include <QListWidgetItem>
#include "CreateComponent.h"

namespace Ui
{
    class ManageComponents;
}

class ComponentModifier;

class KeyboardModifier;


class ManageComponents : public QWidget
{
Q_OBJECT
public:
    explicit ManageComponents(QWidget *parent = 0);

private:
    Ui::ManageComponents *m_ui;
    std::weak_ptr<Component> m_selected;

    ComponentModifier *m_modifier;
    KeyboardModifier *m_keyboardModifier;
    CreateComponent m_createComponent;

private slots:

    void on_listComponents_itemPressed(QListWidgetItem *item);

    void on_buttonCreate_clicked();

    void on_buttonRemove_clicked();

    void componentsChanged(const std::vector<ComponentPtr> &components);

    void on_lineLabel_editingFinished();
};


#endif //OPENCEMULATOR_MANAGECOMPONENTS_H
