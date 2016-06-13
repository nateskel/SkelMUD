//
// Created by nate on 6/12/16.
//

#include "../Skexml/SkexmlParser.h"
#include "../Skexml/Node.h"
#include "SkexmlTest.h"

void SkexmlTest::go()
{
    std::shared_ptr<Node> node = std::make_shared<Node>("Parent");
    node->AddAttribute("AnAttribute", "First");
    node->AddAttribute("AnotherAttribute", "Second");
    node->AddList("ListOfThings", "One;Two;Three");
    std::shared_ptr<Node> childNode = std::make_shared<Node>("ChildNode");
    std::shared_ptr<Node> childNode2 = std::make_shared<Node>("ChildNode2");
    childNode->AddAttribute("ChildAttribute", "CHILD");
    childNode2->AddAttribute("ChildAttribute2", "CHILD2");
    std::shared_ptr<Node> childNode3 = std::make_shared<Node>("ChildNode3");
    childNode3->AddAttribute("ThisIsAttribute", "ASDF");
    childNode3->AddAttribute("AnotherAttribute", "FDSA");
    node->AddChild(childNode);
    node->AddChild(childNode2);
    childNode2->AddChild(childNode3);
    SkexmlParser::BuildSkeXML("/home/nate/SkelMUD/SkelMUD/Skexml/sample.sml", node);
    std::shared_ptr<Node> out_node = SkexmlParser::Parse("/home/nate/SkelMUD/SkelMUD/Skexml/sample.sml");
    Node final = *out_node;
    Node child2 = *final.GetChild("ChildNode2");
}