//
// Created by skelton on 8/26/15.
//

#include "CharacterCreation.h"

CharacterCreator::CharacterCreator(const std::vector<CharacterClass> &classes) : classes(classes) {

}

void CharacterCreator::initializeStates() {
    m_state_map[CreatorState::CLASS] = &CharacterCreator::processClassSelection;
    m_state_map[CreatorState::NAME] = CharacterCreator::processNameSelection;
}

std::string CharacterCreator::processNext(std::string data) {
    // TODO: change this to throw an error, this should never process a state that doesn't exist
    if ( m_state_map.find(m_state) == m_state_map.end())
        return "";
    std::string (* state_function)(std::string) = m_state_map[m_state];
    std::string output = state_function(data);
}

Player *CharacterCreator::getPlayer() {
    return nullptr;
}

std::string CharacterCreator::showClassList() {
    m_state = CreatorState::NAME;
    return "ClassList";
}

std::string CharacterCreator::processClassSelection(std::string data) {
    return "";
}

std::string CharacterCreator::showNameMessage() {
    return "";
}

std::string CharacterCreator::processNameSelection(std::string data) {
    return "";
}

std::string CharacterCreator::signalComplete() {
    return "";
}

