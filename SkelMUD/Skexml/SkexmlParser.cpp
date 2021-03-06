//
// Created by nate on 12/5/15.
//

#include "SkexmlParser.h"
#include <iostream>
#include <algorithm>
#include "../Logger.h"
#include "../Tokenizer.h"
#include "../Utils.h"

std::shared_ptr<Node> SkexmlParser::Parse(std::string filename) {
    std::string input = "";
    std::ifstream file;
    bool valid = false;
    file.open(filename);
    while(!valid and !file.eof()) {
        std::getline(file, input);
        Utils::RemoveEndline(input);
        std::string test = input;
        if (input.length() < 0)
            continue;
        else if (input.substr(0, 1) != "[")
            continue;
        else if (input.find("]") == std::string::npos)
            continue;
        else if (input.length() - 1 > input.find("]"))
            continue;
        valid = true;
    }
    if(!valid)
        return std::make_shared<Node>("");
    std::string node_name = input.substr(1, input.length() - 2);
    std::shared_ptr<Node> node = MakeNode(node_name, file);
    return node;
}

void SkexmlParser::BuildSkeXML(std::string filename, std::shared_ptr<Node> node) {
    std::ofstream file;
    file.open(filename);
    std::stringstream ss;
    WriteNode(ss, node, file);
    file << ss.str();
    file.flush();
    file.close();
}

void SkexmlParser::WriteNode(std::stringstream &xml_string, std::shared_ptr<Node> node, std::ofstream &file) {
    std::string node_name = node->GetName();
    xml_string << "[" << node_name << "]" << "\r\n";
    std::string list = node->GetAttribute("List");
    if(list != "") {
        auto items = Tokenizer::GetAllTokens(list, ';');
        for(auto item : items)
        {
            xml_string << item << "\r\n";
        }
    }
    else {
        std::map<std::string, std::string> attributes = node->GetAttributes();
        for (auto &kv : attributes) {
            std::string name = kv.first;
            if(name=="List")
                continue;
            std::string value = kv.second;
            xml_string << "[" << name << "]" << value << "[/" << name << "]" << "\r\n";
        }
    }
    std::map<std::string, std::shared_ptr<Node>> children = node->GetChildren();
    for(auto &kv : children)
    {
        std::shared_ptr<Node> value = kv.second;
        WriteNode(xml_string, value, file);
    }
    xml_string << "[/" << node_name << "]" << "\r\n";
}

std::shared_ptr<Node> SkexmlParser::MakeNode(std::string name, std::ifstream& file) {
    std::shared_ptr<Node> node = std::make_shared<Node>(name);
    std::string child_name = "";
    std::string input = "";
    std::getline(file, input);
    Utils::RemoveEndline(input);
    while(!file.eof())
    {
        if(input.length() > 1 and input.substr(0,2) == "[/")
            return node;
        else if(input.substr(0,1) != "[") {
            while((input.length() < 2 or input.substr(0,2) != "[/") and !file.eof())
            {
                node->AddListAttribute("List", input);
                std::getline(file, input);
                Utils::RemoveEndline(input);
            }
            if(file.eof() or (input.length() > 1 and input.substr(0,2) == "[/"))
                return node;
        }
        else if(input.length() - 1 > input.find("]")) {
            std::string attribute_name = input.substr(1, input.find("]") - 1);
            unsigned long value_begin = input.find("]") + 1;
            unsigned long value_end = input.find("[/") - value_begin;
            std::string value = input.substr(value_begin, value_end);
            node->AddAttribute(attribute_name, value);
        }
        else {
            child_name = input.substr(1, input.length() - 2);
            node->AddChild(MakeNode(child_name, file));
        }
        std::getline(file, input);
        Utils::RemoveEndline(input);
    }
    return node;
}
