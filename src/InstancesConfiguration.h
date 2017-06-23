#ifndef OPENCEMULATOR_INSTANCESCONFIGURATION_H
#define OPENCEMULATOR_INSTANCESCONFIGURATION_H


#include <vector>
#include "Configuration.h"
#include "Instance.h"

class InstanceConfig {
public:
    InstanceConfig(const std::string &uuid, const std::string &label, std::vector<std::string> &&components) : m_uuid(uuid), m_label(label), m_components(std::move(components)) {
        
    }
    
    inline const std::vector<std::string> &components() const {
        return m_components;
    }
    
    inline const std::string &uuid() const {
        return m_uuid;
    }
    
    inline const std::string &label() const {
        return m_label;
    }
private:
    std::vector<std::string> m_components;
    std::string m_uuid;
    std::string m_label;;
};



class InstancesConfiguration : public Configuration {
public:
    void initialize();
    
    void readConfig(rapidjson::Document &d) override;

    inline const std::vector<InstanceConfig> &instances() {
        return m_instances;
    }
    
    void saveConfig(std::vector<InstancePtr> instances);
private:
    std::vector<InstanceConfig> m_instances;
};


#endif //OPENCEMULATOR_INSTANCESCONFIGURATION_H
