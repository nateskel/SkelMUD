#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H

#include <string>

class OutputManager
{
public:
	OutputManager();
	~OutputManager();
	void SetInputText(std::string inputtext);
	std::string GetIntroText();
protected:
private:
	std::string m_intro_text;
};

class InputManager
{
public:
	InputManager();
	~InputManager();
protected:
private:
};

#endif // TEXTPROCESSOR_H