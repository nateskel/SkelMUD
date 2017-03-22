//
// Created by skelton on 12/22/16.
//

#ifndef SKELMUD_CONFIGURATION_H
#define SKELMUD_CONFIGURATION_H


#include <string>

class Configuration {
private:
    int m_port;
    std::string m_data_directory;
public:
    void LoadConfig(std::string filename);
    void LoadConfig();
    int GetPort();
    void SetPort(int port);
};


#endif //SKELMUD_CONFIGURATION_H
