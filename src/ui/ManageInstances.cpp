#include <OpenCEmulator.h>
#include <iostream>
#include "ManageInstances.h"
#include "ui_manageinstances.h"

ManageInstances::ManageInstances(QWidget *parent) : QWidget(parent), m_ui(new Ui::ManageInstances) {
    m_ui->setupUi(this);

    connect(&OpenCEmulator::get(), &OpenCEmulator::instancesChanged, this, &ManageInstances::instancesChanged);
}



void ManageInstances::on_buttonRemove_clicked() {
    if (auto instance = m_instance.lock()) {
        OpenCEmulator::get().removeInstance(instance);
    }
}



void ManageInstances::on_buttonDetach_clicked() {
    if (auto instance = m_instance.lock()) {
        for (QListWidgetItem *item : m_ui->listAttached->selectedItems()) {
            if (auto component = item->data(Qt::UserRole).value<ComponentWeakPtr>().lock()) {
                component->detach(instance);
                instance->detachComponent(component);
            }
        }
    }
    
    refresh();
}



void ManageInstances::on_buttonAttach_clicked() {
    if (auto instance = m_instance.lock()) {
        for (QListWidgetItem *item : m_ui->listUnattached->selectedItems()) {
            if (auto component = item->data(Qt::UserRole).value<ComponentWeakPtr>().lock()) {
                component->attach(instance);
                instance->attachComponent(component);
            }
        }
    }

    refresh();
}



void ManageInstances::on_buttonPower_clicked() {
    if (auto instance = m_instance.lock()) {
        instance->stop();
    }
}



void ManageInstances::on_buttonCreate_clicked() {
    InstancePtr instance = std::make_shared<Instance>(1024 * 1024 * 4);
    
    OpenCEmulator::get().addInstance(instance);
}



void ManageInstances::on_buttonReboot_clicked() {
    if (auto instance = m_instance.lock()) {
        instance->stop();
        instance->start();
    }
}



Q_DECLARE_METATYPE(InstanceWeakPtr)
void ManageInstances::instancesChanged(const std::vector<InstancePtr> &instances) {
    m_ui->listInstances->clear();

    for (const InstancePtr &instance : instances) {
        // This is Qt so the item's memory should be handled by Qt after setting the parent
        QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(instance->label()), m_ui->listInstances);
        item->setData(Qt::UserRole, QVariant::fromValue(InstanceWeakPtr(instance)));
    }
}



Q_DECLARE_METATYPE(ComponentWeakPtr);
void ManageInstances::on_listInstances_currentItemChanged(QListWidgetItem *current) {
    if (current) {
        m_instance = current->data(Qt::UserRole).value<InstanceWeakPtr>();
        refresh();
    } else {
        m_instance.reset();
    }
}



void ManageInstances::refresh() {
    if (auto instance = m_instance.lock()) {
        // populate components lists
        
        m_ui->labelSelected->setText(QString::fromStdString(instance->address()));
        m_ui->lineLabel->setText(QString::fromStdString(instance->label()));

        m_ui->listAttached->clear();
        m_ui->listUnattached->clear();

        auto &attachedComponents = instance->components();

        for (const ComponentPtr &component : OpenCEmulator::get().components()) {
            if (std::find_if(attachedComponents.begin(), attachedComponents.end(),
                             [component](ComponentWeakPtr &c) { return c.lock() == component; }) ==
                attachedComponents.end()) {
                QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(component->label() + " (" + component->type() + ")"),
                                                            m_ui->listUnattached);

                item->setData(Qt::UserRole, QVariant::fromValue(ComponentWeakPtr(component)));
            }
        }

        for (ComponentWeakPtr &c : attachedComponents) {
            if (auto component = c.lock()) {
                QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(component->label() + " (" + component->type() + ")"),
                                                            m_ui->listAttached);
                item->setData(Qt::UserRole, QVariant::fromValue(ComponentWeakPtr(component)));
            }
        }
    }
}

void ManageInstances::on_lineLabel_editingFinished() {
    if (auto instance = m_instance.lock()) {
        instance->setLabel(m_ui->lineLabel->text().toStdString());
        
        emit OpenCEmulator::get().instancesChanged(OpenCEmulator::get().instances());
    }
}
