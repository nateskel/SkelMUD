//
// Created by nate on 12/5/15.
//

#ifndef SKELMUD_NODE_H
#define SKELMUD_NODE_H


#include <memory>
#include <map>
#include <string>
#include <vector>

class Node {
public:
    Node();
    Node(const std::string &_name) : _name(_name) { }
    Node(const std::string &_name, const bool is_list) : _name(_name), _is_list(is_list) { }
    std::shared_ptr<Node> GetParent();
    void SetParent(std::shared_ptr<Node> parent);
    std::shared_ptr<Node> GetChild(std::string child_name);
    void AddChild(std::shared_ptr<Node> child);
    void AddChild(std::string name, std::shared_ptr<Node> child);
    std::string GetAttribute(std::string attribute_name);
    std::vector<std::string> GetListAttribute(std::string attribute_name);
    void AddAttribute(std::string name, std::string value);
    void AddListAttribute(std::string name, std::string value);
    void AddList(std::string name, std::string value);
    void AddList(std::string name, std::vector<std::string> attribute_list);
    std::string GetName();
    void SetName(std::string name);

    // Getters
    const std::map<std::string, std::shared_ptr<Node>> &GetChildren() const {
        return _children;
    }

    const std::map<std::string, std::string> &GetAttributes() const {
        return _attributes;
    }

    const bool &IsList() const {
        return _is_list;
    }

protected:
    std::shared_ptr<Node> _parent;
    std::map<std::string, std::shared_ptr<Node>> _children;
    std::map<std::string, std::string> _attributes;
    std::string _name;
    bool _is_list;
};


#endif //SKELMUD_NODE_H
