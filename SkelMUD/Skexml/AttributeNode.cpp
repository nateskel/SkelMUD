//
// Created by nate on 12/5/15.
//

#include "AttributeNode.h"

void AttributeNode::AddAttribute(std::string attribute_name, std::string data) {
    _attributes[attribute_name] = data;
}

std::map<std::string, std::string> AttributeNode::GetAttributes() {
    return _attributes;
}
