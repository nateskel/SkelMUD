//
// Created by skelton on 12/22/16.
//
//TODO: Linux specific
#include <unistd.h>
//////////////////////
#include "Configuration.h"
#include "../Skexml/SkexmlParser.h"
#include "../GameData.h"
#include "../Logger.h"

void Configuration::LoadConfig(std::string filename) {
    std::shared_ptr<Node> root = SkexmlParser::Parse(filename);
    m_port = std::atoi(root->GetAttribute("Port").c_str());
    m_data_directory = root->GetAttribute("DataDirectory");
    if(m_data_directory == "") {
        // TODO: this is linux specific
        m_data_directory = get_current_dir_name();
    }
    std::stringstream ss;
    ss << "PORT: " << m_port;
    Logger::Debug(ss.str());
}

void Configuration::LoadConfig() {
    LoadConfig(GameData::CONFIG_FILE);
}

int Configuration::GetPort() {
    return m_port;
}

void Configuration::SetPort(int port) {
    m_port = port;
}