#include "Game.h"
#include "Skexml/SkexmlParser.h"
#include "Skexml/Node.h"
#include "Test/SkexmlTest.h"

int main() {
    Logger::SetOutputLevel(Logger::L_DEBUG);
//    SkexmlParser parser;
//    std::shared_ptr<Node> node = parser.Parse("/home/skelton/SkelMUD/SkelMUD/SkelMUD/Skexml/sample.sml");
//    Node* n = node.get();
//    Node asdf = *n;
//    Node child = *asdf.GetChild("List_of_stuff");
//    std::string test = child.GetAttribute("List");
    SkexmlTest::go();
//    Game game = Game();
//    Logger::Info("Starting...");
//    game.Start();
    return 0;
}