//
// Created by nate on 12/5/15.
//

#include "Node.h"
#include "../Tokenizer.h"

Node::Node() {
    _parent = nullptr;
    _name = "";
    _is_list = false;
}

std::shared_ptr<Node> Node::GetChild(std::string child_name) {
    auto node = _children[child_name];
    if(node == nullptr)
        return nullptr;
    if(node->IsList())
        return nullptr;
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

void Node::AddList(std::string name, std::string value) {
    std::shared_ptr<Node> list_node = std::make_shared<Node>(name, true);
    list_node->AddAttribute("List", value);
    this->AddChild(list_node);
}

void Node::AddList(std::string name, std::vector<std::string> attribute_list) {
    std::shared_ptr<Node> list_node = std::make_shared<Node>(name, true);
    std::string list_string = "";
    for(auto item: attribute_list) {
        list_string.append(item);
        list_string.append(";");
    }
    list_node->AddAttribute("List", list_string);
    this->AddChild(list_node);
}

void Node::AddList(std::string name, std::vector<int> attribute_list) {
    std::vector<std::string> attribute_list_strings;
    for(auto item : attribute_list) {
        attribute_list_strings.push_back(std::to_string(item));
    }
    AddList(name, attribute_list_strings);
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

std::vector<std::string> Node::GetListAttribute(std::string attribute_name) {
    std::shared_ptr<Node> node;
    if(_children.find(attribute_name) != _children.end())
        node = _children[attribute_name];
    else
        return std::vector<std::string>();
    std::string list_attribute = node->GetAttribute("List");
    return Tokenizer::GetAllTokens(list_attribute, ';');
}
