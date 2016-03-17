//
// Created by nate on 12/5/15.
//

#include "Node.h"

Node::Node() {
    _parent = nullptr;
}

std::shared_ptr<Node> Node::GetChild(std::string child_name) {
    return _children[child_name];
}

void Node::AddChild(std::string name, std::shared_ptr<Node> child) {
    child->SetName(name);
    _children[name] = child;
}

void Node::AddChild(std::shared_ptr<Node> child) {
    _children[child->GetName()] = child;
}

void Node::AddAttribute(std::string name, std::string value) {
    _attributes[name] = value;
}


void Node::AddListAttribute(std::string name, std::string value) {
    if(_attributes.find(name) == _attributes.end()) {
        _attributes[name] = value;
    }
    else {
        _attributes[name] = _attributes[name] + ";" + value;
    }
}

std::shared_ptr<Node> Node::GetParent() {
    return std::shared_ptr<Node>();
}

void Node::SetParent(std::shared_ptr<Node> parent) {
    _parent = parent;
}

void Node::SetName(std::string name) {
    _name = name;
}

std::string Node::GetName() {
    return _name;
}

std::string Node::GetAttribute(std::string attribute_name) {
    if(_attributes.find(attribute_name) != _attributes.end())
        return _attributes[attribute_name];
    else
        return "";
}
