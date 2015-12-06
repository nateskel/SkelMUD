//
// Created by nate on 12/5/15.
//

#ifndef SKELMUD_ATTRIBUTENODE_H
#define SKELMUD_ATTRIBUTENODE_H


#include "Node.h"

class AttributeNode : public Node {
public:
    void AddAttribute(std::string attribute_name, std::string data);
    std::map<std::string, std::string> GetAttributes();
private:
    std::map<std::string, std::string> _attributes;
};


#endif //SKELMUD_ATTRIBUTENODE_H
