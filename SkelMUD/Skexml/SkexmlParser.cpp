//
// Created by nate on 12/5/15.
//

#include "SkexmlParser.h"
#include <iostream>
#include <fstream>
#include "../Logger.h"

std::shared_ptr<Node> SkexmlParser::Parse(std::string filename) {
    std::string input = "";
    std::ifstream file;
    bool valid = true;
    file.open(filename);
    file >> input;
    if(input.length() < 0)
        valid = false;
    else if(input.substr(0,1) != "[")
        valid = false;
    else if(input.find("]") == std::string::npos)
        valid = false;
    else if(input.length() - 1 > input.find("]"))
        valid = false;
    if(!valid)
        return std::shared_ptr<Node>();
    std::string node_name = input.substr(1, input.length() - 2);
    std::shared_ptr<Node> node = MakeNode(node_name, file);
    return node;
}

void SkexmlParser::BuildSkeXML(std::string filename, std::shared_ptr<Node> node) {

}

std::shared_ptr<Node> SkexmlParser::MakeNode(std::string name, std::ifstream& file) {
    std::shared_ptr<Node> node = std::make_shared<Node>(name);
    std::string child_name = "";
    Logger::SetOutputLevel(Logger::L_DEBUG);
    Logger::Debug(name);
    std::string input = "";
    file >> input;
    while(!file.eof())
    {
        if(input.length() > 1 and input.substr(0,2) == "[/")
            return node;
        else if(input.substr(0,1) != "[") {
            while((input.length() < 2 or input.substr(0,2) != "[/") and !file.eof())
            {
                node->AddListAttribute("List", input);
                file >> input;
            }
            if(file.eof())
                return node;
        }
        else if(input.length() - 1 > input.find("]")) {
            std::string attribute_name = input.substr(1, input.find("]") - 1);
            unsigned long value_begin = input.find("]");
            unsigned long value_end = input.find("[/");
            std::string value = input.substr(value_begin, value_end);
            node->AddAttribute(attribute_name, value);
        }
        else {
            child_name = input.substr(1, input.length() - 2);
            node->AddChild(MakeNode(child_name, file));
        }
        file >> input;
    }
    return node;
}
