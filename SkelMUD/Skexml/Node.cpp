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
