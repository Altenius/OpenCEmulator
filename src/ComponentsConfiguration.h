#ifndef OPENCEMULATOR_COMPONENTSCONFIGURATIOHN_H
#define OPENCEMULATOR_COMPONENTSCONFIGURATIOHN_H


#include <vector>
#include <components/Component.h>
#include "Configuration.h"

class ComponentConfig {
public:
    ComponentConfig(const std::string &name, const std::string &type, const std::string &label = std::string(), const std::string &extra = std::string()) : m_name(name), m_type(type), m_extra(extra), m_label(label) {
        
    }
    
    const std::string &name() const {
        return m_name;
    }
    
    const std::string &type() const {
        return m_type;
    }
    
    const std::string &extra() const {
        return m_extra;
    }
    
    const std::string &label() const {
        return m_label;
    }
private:
    std::string m_name;
    std::string m_type;
    std::string m_extra;
    std::string m_label;
};



class ComponentsConfiguration : public Configuration {
public:
    void initialize();

    void readConfig(rapidjson::Document &d) override;
    
    const std::vector<ComponentConfig> &components() {
        return m_components;
    }
    
    void saveConfig(const std::vector<ComponentPtr> &components);
private:
    std::vector<ComponentConfig> m_components;
};


#endif //OPENCEMULATOR_COMPONENTSCONFIGURATIOHN_H
