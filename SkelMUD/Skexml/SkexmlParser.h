//
// Created by nate on 12/5/15.
//

#ifndef SKELMUD_SKEXMLPARSER_H
#define SKELMUD_SKEXMLPARSER_H


#include <memory>
#include "Node.h"

class SkexmlParser {
public:
    static std::shared_ptr<Node> Parse(std::string filename);
    static void BuildSkeXML(std::string filename, std::shared_ptr<Node>);
private:

};


#endif //SKELMUD_SKEXMLPARSER_H
