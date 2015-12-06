//
// Created by nate on 12/5/15.
//

#include "ListNode.h"

ListNode::ListNode(const std::string &_name) : Node(_name) {

}

std::list<std::string> ListNode::GetList() {
    return _list_attribute;
}

void ListNode::SetList(std::list<std::string> list) {
    _list_attribute = list;
}

void ListNode::Add(std::string string) {
    _list_attribute.push_back(string);
}

std::string ListNode::Get(int index) {
    // Not Yet Implemented
}
