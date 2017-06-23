#ifndef OPENCEMULATOR_COMPONENT_H
#define OPENCEMULATOR_COMPONENT_H

#include <memory>
#include <QVector>
#include <QString>
#include <unordered_map>
#include <ArgList.h>
#include <functional>

class Component;
typedef std::shared_ptr<Component> ComponentPtr;
typedef std::weak_ptr<Component> ComponentWeakPtr;

class Instance;
typedef std::shared_ptr<Instance> InstancePtr;
typedef std::weak_ptr<Instance> InstanceWeakPtr;

typedef std::function<bool(const ArgList &, ArgList &)> ComponentMethod;



struct MethodInfo {
    std::string doc;
    bool direct, getter, setter;
    
    MethodInfo(std::string doc_, bool direct_, bool getter_, bool setter_) : doc(doc_), direct(direct_), getter(getter_), setter(setter_) {};
};



class Component {
public:
    Component(const std::string &type, const std::string &uuid = std::string(), const std::string &label = std::string());
    
    void attach(InstancePtr instance);
    void detach(InstancePtr instance);
    void detach(Instance *instance);
    
    inline const std::string &uuid() {
        return m_uuid;
    }
    
    inline void setLabel(const std::string &label) {
        m_label = label;
    }
    
    inline const std::string &type() {
        return m_type;
    }
    
    inline const std::string &label() {
        if (m_label.empty()) {
            return m_uuid;
        }
        return m_label;
    }
    
    virtual void cleanup(); // detach from all instances and stuff

    inline std::unordered_map<std::string, ComponentMethod> &methods() {
        return m_methods;
    }
    
    inline std::unordered_map<std::string, MethodInfo> &methodsinfo() {
        return m_methodsinfo;
    };
    
protected:
    inline void registerMethod(std::string name, ComponentMethod method, std::string doc, bool direct = false, bool getter = false, bool setter = false) {
        m_methods.insert(std::make_pair(name, method));
        m_methodsinfo.insert(std::make_pair(name, MethodInfo(doc, direct, getter, setter)));
    }
    
    QVector<InstanceWeakPtr> m_instances; // attached instances
    std::string m_uuid, m_type, m_label;
    
private:
    std::unordered_map<std::string, ComponentMethod> m_methods;
    std::unordered_map<std::string, MethodInfo> m_methodsinfo;
};


#endif //OPENCEMULATOR_COMPONENT_H
