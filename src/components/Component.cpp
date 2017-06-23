#include <QUuid>
#include "Component.h"
#include "../Instance.h"

void Component::attach(InstancePtr instance) {
    for (InstanceWeakPtr i : m_instances) {
        if (i.lock() == instance) {
            return;
        }
    }
    
    m_instances.append(InstanceWeakPtr(instance));
}



void Component::detach(InstancePtr instance) {
    for (auto it = m_instances.begin(); it != m_instances.end(); ++it) {
        if (it->lock() == instance) {
            m_instances.erase(it);
            return;
        }
    }
}



void Component::detach(Instance *instance) {
    for (auto it = m_instances.begin(); it != m_instances.end(); ++it) {
        if (it->lock().get() == instance) {
            m_instances.erase(it);
            return;
        }
    }
}



Component::Component(const std::string &type, const std::string &uuid, const std::string &label) : m_type(type), m_uuid(uuid), m_label(label) {
    if (m_uuid.empty()) {
        m_uuid = QUuid::createUuid().toString().toStdString();
        m_uuid.erase(m_uuid.begin());
        m_uuid.erase(m_uuid.end() - 1);
    }
}



void Component::cleanup() {
    for (InstanceWeakPtr i : m_instances) {
        if (auto instance = i.lock()) {
            instance->detachComponent(this);
        }
    }
}