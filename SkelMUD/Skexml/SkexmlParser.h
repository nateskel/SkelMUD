//
// Created by nate on 12/5/15.
//

#ifndef SKELMUD_SKEXMLPARSER_H
#define SKELMUD_SKEXMLPARSER_H


#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include "Node.h"

class SkexmlParser {
public:
    static std::shared_ptr<Node> Parse(std::string filename);
    static void BuildSkeXML(std::string filename, std::shared_ptr<Node> node);
private:
    static std::shared_ptr<Node> MakeNode(std::string name, std::ifstream& file);
    static void WriteNode(std::stringstream &xml_string, std::shared_ptr<Node> node,
                                        std::ofstream &file);

};


#endif //SKELMUD_SKEXMLPARSER_H
