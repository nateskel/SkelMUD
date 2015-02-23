#ifndef FILE_H
#define FILE_H

#include "Connection.h"
#include "Planet.h"
#include <string>
#include <fstream>
#include <vector>

class File
{
public:
	File::File();
	File::~File();
	bool ReadNextLine(std::string &data);
	void WriteLine(std::string data);
	void OpenRead(std::string filename);
	void OpenWrite(std::string filename);
	void OpenReadWrite(std::string filename);
	void Close();
	std::vector<Connection::Account> LoadAccounts();
	std::vector<Planet*> LoadPlanets();
protected:
private:
	const std::string DIRECTORY = "D:\\SkelMUD\\SkelMUD\\SkelMUD\\Planets\\";
	std::string m_filename;
	std::fstream m_file;
};
#endif // FILE_H