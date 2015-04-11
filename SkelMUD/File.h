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
	File();
	~File();
	bool ReadNextLine(std::fstream& stream, std::string &data);
	void WriteLine(std::string data);
	void OpenRead(std::fstream& stream, std::string filename);
	void OpenWrite(std::string filename);
	void OpenReadWrite(std::string filename);
	void Close(std::fstream& stream);
	std::vector<Connection::Account> LoadAccounts();
	std::vector<Planet*> LoadPlanets();
protected:
private:
	// Temporary hardcoded value
	// possibly move to a config file
#ifdef _WIN32
	const std::string DIRECTORY = "D:\\SkelMUD\\SkelMUD\\SkelMUD\\Planets\\";
#else
	const std::string DIRECTORY = "/root/SkelMUD/SkelMUD/Planets/";
#endif
	std::string m_filename;
	std::fstream *m_file;
};
#endif // FILE_H