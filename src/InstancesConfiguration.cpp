#include "OpenCEmulator.h"
#include <iostream>
#include <fstream>
#include "InstancesConfiguration.h"



void InstancesConfiguration::initialize() {
    if (!load(OpenCEmulator::get().baseDirectory() + "/instances.json")) {
        std::ofstream file(OpenCEmulator::get().baseDirectory() + "/instances.json");
        if (!file.is_open()) {
            std::cerr << "Failed to open instance.json" << std::endl;
            return;
        }
        file << "{}";
        file.close();
    }
}



void InstancesConfiguration::readConfig(rapidjson::Document &d) {
    for (auto &v : d.GetObject()) {
        if (!v.value.IsObject()) {
            std::cerr << "error: non-object located in instances array in instances.json" << std::endl;
            continue;
        }
        
        std::vector<std::string> components;
        std::string label;
        
        for (auto &member : v.value.GetObject()) {
            std::string name = member.name.GetString();
            if (name == "components") {
                if (!member.value.IsArray()) {
                    std::cerr << "error: 'components' in an instance in instances.json is not an array" << std::endl;
                    continue;
                }
                
                for (auto &component : member.value.GetArray()) {
                    if (!component.IsString()) {
                        std::cerr << "error: component in component section of an instance in instances.json is not a string" << std::endl;
                        continue;
                    }
                    components.push_back(component.GetString());
                }
            } else if (name == "label") {
                if (member.value.IsString()) {
                    label.assign(member.value.GetString());
                }
            }
        }
        
        m_instances.push_back(InstanceConfig(v.name.GetString(), label, std::move(components)));
    }
}



void InstancesConfiguration::saveConfig(std::vector<InstancePtr> instances) {
    rapidjson::Document d(rapidjson::kObjectType);
    
    for (InstancePtr &instance : instances) {
        rapidjson::Value instanceData(rapidjson::kObjectType);
        
        rapidjson::Value components(rapidjson::kArrayType);
        for (ComponentWeakPtr c : instance->components()) {
            if (auto component = c.lock()) {
                if (component->type() == "computer") {
                    // note: this will break connecting to other computers
                    continue;
                }
                rapidjson::Value uuid(rapidjson::kStringType);
                std::string sUuid = component->uuid();
                uuid.SetString(sUuid.c_str(), sUuid.length(), d.GetAllocator());
                components.PushBack(uuid.Move(), d.GetAllocator());
            }
        }
        
        instanceData.AddMember("components", components, d.GetAllocator());

        const std::string &sLabel = instance->label();
        if (!sLabel.empty()) {
            rapidjson::Value label(rapidjson::kStringType);
            label.SetString(sLabel.c_str(), sLabel.length(), d.GetAllocator());
            instanceData.AddMember("label", label, d.GetAllocator());
        }
        
        rapidjson::Value uuid(rapidjson::kStringType);
        const std::string &sUuid = instance->address();
        uuid.SetString(sUuid.c_str(), sUuid.length(), d.GetAllocator());
        
        
        d.AddMember(uuid, instanceData, d.GetAllocator());
    }

    if (!save(OpenCEmulator::get().baseDirectory() + "/instances.json", d)) {
        std::cerr << "failed to save instances.json" << std::endl;
    }
}
