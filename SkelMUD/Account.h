#ifndef ACCOUNT_H
#define ACCOUNT_H

#include<string>
#include<vector>
#include "Player.h"

class Account
{
public:
	Account();
	~Account();
protected:
private:
	enum AccountLevel
	{
		Wizard, GM, Standard, Trial
	};

	AccountLevel m_level;
	std::string m_username;
	std::string m_password;
	vector<std::string> m_characters;
};

#endif // ACCOUNT_H