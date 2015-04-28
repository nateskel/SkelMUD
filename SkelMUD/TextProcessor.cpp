#include "TextProcessor.h"

OutputManager::OutputManager() {
    m_intro_text = "Welcome to SkelMUD!\r\n\r\nEnter Username:";
}

OutputManager::~OutputManager() {

}

void OutputManager::SetInputText(std::string inputtext) {
    m_intro_text = inputtext;
}

std::string OutputManager::GetIntroText() {
    return m_intro_text;
}