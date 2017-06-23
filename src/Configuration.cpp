#include "Configuration.h"
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/istreamwrapper.h>
#include <fstream>
#include <iostream>

Configuration::Configuration() {

}



bool Configuration::load(const std::string &filename) {
    std::ifstream file(filename); // note: fopen is faster with rapidJSON
    if (!file.is_open()) {
        std::cerr << "Failed to load configuration file: " << filename << std::endl;
        return false;
    }
    
    rapidjson::IStreamWrapper isw(file);

    rapidjson::Document d;
    d.ParseStream(isw);
    file.close();
    
    readConfig(d);
    return true;
}



bool Configuration::save(const std::string &filename, const rapidjson::Document &d) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to save configuration file: " << filename << std::endl;
        return false;
    }

    rapidjson::OStreamWrapper osw(file);
    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
    d.Accept(writer);
    file.close();
    return true;
}
