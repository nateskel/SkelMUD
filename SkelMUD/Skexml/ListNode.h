//
// Created by nate on 12/5/15.
//

#ifndef SKELMUD_LISTNODE_H
#define SKELMUD_LISTNODE_H


#include <bits/stl_list.h>
#include "Node.h"

class ListNode : public Node{
public:
    // ListNode(const std::list<std::string> &list_attribute) : _list_attribute(list_attribute) { }

    ListNode(const std::string &_name);

    std::list<std::string> GetList();
    void SetList(std::list<std::string> list);
    void Add(std::string);
    std::string Get(int index);
private:
    std::list<std::string> _list_attribute;
};


#endif //SKELMUD_LISTNODE_H
