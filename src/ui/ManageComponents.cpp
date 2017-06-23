#include <components/Component.h>
#include <OpenCEmulator.h>
#include "ManageComponents.h"
#include "ui_managecomponents.h"
#include "ComponentModifier.h"
#include "KeyboardModifier.h"



ManageComponents::ManageComponents(QWidget *parent) : QWidget(parent), m_ui(new Ui::ManageComponents),
                                                      m_modifier(nullptr)
{
    m_ui->setupUi(this);

    m_keyboardModifier = new KeyboardModifier(m_ui->frameExtraModify);
    m_keyboardModifier->hide();

    connect(&OpenCEmulator::get(), &OpenCEmulator::componentsChanged, this, &ManageComponents::componentsChanged);
}



Q_DECLARE_METATYPE(ComponentWeakPtr);



void ManageComponents::on_listComponents_itemPressed(QListWidgetItem *item)
{
    m_selected = item->data(Qt::UserRole).value<ComponentWeakPtr>();

    if (auto component = m_selected.lock()) {
        std::string type = component->type();

        m_ui->labelSelected->setText(QString::fromStdString(component->uuid()));
        m_ui->lineLabel->setText(QString::fromStdString(component->label()));

        if (m_modifier != nullptr) {
            m_modifier->hide();
        }

        if (type == "keyboard") {
            m_modifier = m_keyboardModifier;
            m_modifier->update(component);
            m_modifier->show();
        }
    }
}



void ManageComponents::componentsChanged(const std::vector<ComponentPtr> &components)
{
    m_ui->listComponents->clear();

    for (const ComponentPtr &component : components) {
        QListWidgetItem *item = new QListWidgetItem(
                QString::fromStdString(component->label() + " (" + component->type() + ")"), m_ui->listComponents);
        item->setData(Qt::UserRole, QVariant::fromValue(ComponentWeakPtr(component)));
    }
}



void ManageComponents::on_buttonCreate_clicked()
{
    m_createComponent.show();
}



void ManageComponents::on_buttonRemove_clicked()
{
    if (auto component = m_selected.lock()) {
        OpenCEmulator::get().removeComponent(component);
    }
}



void ManageComponents::on_lineLabel_editingFinished()
{
    if (auto component = m_selected.lock()) {
        component->setLabel(m_ui->lineLabel->text().toStdString());

        emit OpenCEmulator::get().componentsChanged(OpenCEmulator::get().components());
    }
}
