//
// Created by skelton on 12/22/16.
//

#include "Configuration.h"
#include "../Skexml/SkexmlParser.h"
#include "../GameData.h"
#include "../Logger.h"

void Configuration::LoadConfig(std::string filename) {
    std::shared_ptr<Node> root = SkexmlParser::Parse(filename);
    m_port = std::atoi(root->GetAttribute("Port").c_str());
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