#include <QUuid>
#include "OpenCEmulator.h"
#include <iostream>
#include "Component.h"
#include "Instance.h"
#include "ComponentEEPROM.h"
#include "ComponentRealFilesystem.h"
#include "ComponentGPU.h"
#include "ComponentKeyboard.h"
#include "ComponentScreen.h"



void Component::attach(InstancePtr instance)
{
    for (InstanceWeakPtr i : m_instances) {
        if (i.lock() == instance) {
            return;
        }
    }

    m_instances.append(InstanceWeakPtr(instance));
}



void Component::detach(InstancePtr instance)
{
    for (auto it = m_instances.begin(); it != m_instances.end(); ++it) {
        if (it->lock() == instance) {
            m_instances.erase(it);
            return;
        }
    }
}



void Component::detach(Instance *instance)
{
    for (auto it = m_instances.begin(); it != m_instances.end(); ++it) {
        if (it->lock().get() == instance) {
            m_instances.erase(it);
            return;
        }
    }
}



Component::Component(const std::string &type, const std::string &uuid, const std::string &label) : m_type(type),
                                                                                                   m_uuid(uuid),
                                                                                                   m_label(label)
{
    if (m_uuid.empty()) {
        m_uuid = QUuid::createUuid().toString().toStdString();
        m_uuid.erase(m_uuid.begin());
        m_uuid.erase(m_uuid.end() - 1);
    }
}



void Component::cleanup()
{
    for (InstanceWeakPtr i : m_instances) {
        if (auto instance = i.lock()) {
            instance->detachComponent(this);
        }
    }
}



ComponentPtr Component::create(const std::string &type, const std::string &uuid, const std::string &label,
                               const std::string &data)
{
    ComponentPtr component;

    if (type == "eeprom") {
        component.reset(new ComponentEEPROM(uuid, label));
    } else if (type == "filesystem") {
        component.reset(new ComponentRealFilesystem(uuid, label));
    } else if (type == "gpu") {
        component.reset(new ComponentGPU(uuid, label));
    } else if (type == "keyboard") {
        ComponentPtr screen;
        screen = OpenCEmulator::get().findComponent(data);
        if (!screen) {
            std::cerr << "could not find screen with address " << data << " to attach to keyboard with address " << uuid
                      << std::endl;
        }

        component.reset(new ComponentKeyboard(screen, uuid, label));

        if (screen) {
            std::static_pointer_cast<ComponentScreen>(screen)->attachKeyboard(component);
        }
    } else if (type == "screen") {
        component.reset(new ComponentScreen(uuid, label));
    } else {
        std::cerr << "unable to create component with unknown type: " << type << std::endl;
        // return ComponentPtr();
    }

    if (component) {
        OpenCEmulator::get().addComponent(component);
        std::cout << "added component " << type << " with uuid " << uuid << std::endl;
    }
    return component;
}