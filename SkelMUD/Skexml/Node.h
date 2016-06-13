//
// Created by nate on 12/5/15.
//

#ifndef SKELMUD_NODE_H
#define SKELMUD_NODE_H


#include <memory>
#include <map>
#include <string>

class Node {
public:
    Node();
    Node(const std::string &_name) : _name(_name) { }
    std::shared_ptr<Node> GetParent();
    void SetParent(std::shared_ptr<Node> parent);
    std::shared_ptr<Node> GetChild(std::string child_name);
    void AddChild(std::shared_ptr<Node> child);
    void AddChild(std::string name, std::shared_ptr<Node> child);
    std::string GetAttribute(std::string attribute_name);
    void AddAttribute(std::string name, std::string value);
    void AddListAttribute(std::string name, std::string value);
    void AddList(std::string name, std::string value);
    std::string GetName();
    void SetName(std::string name);

    // Getters
    const std::map<std::string, std::shared_ptr<Node>> &GetChildren() const {
        return _children;
    }

    const std::map<std::string, std::string> &GetAttributes() const {
        return _attributes;
    }

protected:
    std::shared_ptr<Node> _parent;
    std::map<std::string, std::shared_ptr<Node>> _children;
    std::map<std::string, std::string> _attributes;
    std::string _name;
};


#endif //SKELMUD_NODE_H
