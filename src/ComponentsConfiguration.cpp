#include "ComponentsConfiguration.h"
#include "OpenCEmulator.h"
#include <fstream>
#include <iostream>
#include "components/ComponentScreen.h"

void ComponentsConfiguration::initialize() {
    if (!load(OpenCEmulator::get().baseDirectory() + "/components.json")) {
        std::ofstream file(OpenCEmulator::get().baseDirectory() + "/components.json");
        if (!file.is_open()) {
            std::cerr << "Failed to open components.json" << std::endl;
            return;
        }
        file << "{}";
        file.close();
    }
}



void ComponentsConfiguration::readConfig(rapidjson::Document &d) {
    for (auto &component : d.GetObject()) {
        if (!component.value.IsObject()) {
            std::cerr << "error: non-object located in root object of components.json" << std::endl;
            continue;
        }

        auto type = component.value.FindMember("type");
        if (type == component.value.MemberEnd() || !type->value.IsString()) {
            std::cerr << "error: could not find 'type' string in a component in components.json" << std::endl;
            continue;
        }
        
        std::string extraString, labelString;
        
        auto extra = component.value.FindMember("extra");
        if (extra != component.value.MemberEnd() && extra->value.IsString()) {
            extraString.assign(extra->value.GetString());
        }
        
        auto label = component.value.FindMember("label");
        if (label != component.value.MemberEnd() && label->value.IsString()) {
            labelString.assign(label->value.GetString());
        }

        m_components.push_back(ComponentConfig(component.name.GetString(), type->value.GetString(), labelString, extraString));
    }
}



void ComponentsConfiguration::saveConfig(const std::vector<ComponentPtr> &components) {
    rapidjson::Document d(rapidjson::kObjectType);
    
    for (const ComponentPtr &component : components) {
        rapidjson::Value componentData(rapidjson::kObjectType);
        
        rapidjson::Value type(rapidjson::kStringType);
        std::string sType = component->type();
        if (sType == "computer") {
            continue;
        }
        
        std::string extra;
        
        if (sType == "keyboard") {
            // save data about the connected screen
            extra = std::static_pointer_cast<ComponentKeyboard>(component)->screen()->uuid();
        }
        type.SetString(sType.c_str(), sType.length(), d.GetAllocator());
        componentData.AddMember("type", type, d.GetAllocator());
        
        if (!extra.empty()) {
            rapidjson::Value extraV(rapidjson::kStringType);
            extraV.SetString(extra.c_str(), extra.length(), d.GetAllocator());
            componentData.AddMember("extra", extraV, d.GetAllocator());
        }
        
        const std::string &label = component->label();
        if (!label.empty()) {
            rapidjson::Value labelV(rapidjson::kStringType);
            labelV.SetString(label.c_str(), label.length(), d.GetAllocator());
            componentData.AddMember("label", labelV, d.GetAllocator());
        }
        
        rapidjson::Value uuid(rapidjson::kStringType);
        std::string sUuid = component->uuid();
        uuid.SetString(sUuid.c_str(), sUuid.length(), d.GetAllocator());
        d.AddMember(uuid, componentData, d.GetAllocator());
    }
    
    if (!save(OpenCEmulator::get().baseDirectory() + "/components.json", d)) {
        std::cerr << "failed to save components.json" << std::endl;
    }
}
