#ifndef FILE_H
#define FILE_H

#include "Connection.h"
#include "Planet.h"
#include <string>
#include <fstream>
#include <vector>

class File {
public:
    File();

    ~File();

    bool ReadNextLine(std::fstream &stream, std::string &data);

    void WriteLine(std::fstream &stream, std::string data);

    void OpenRead(std::fstream &stream, std::string filename);

    void OpenWrite(std::fstream &stream, std::string filename);

    void OpenReadWrite(std::string filename);

    void Close(std::fstream &stream);

    std::vector<Connection::Account> LoadAccounts();

    void SaveAccount(std::fstream &stream, Connection::Account account);

    std::vector<Planet*> LoadPlanets();

    std::vector<CharacterClass> LoadClasses();

protected:
private:
    // Temporary hardcoded value
    // possibly move to a config file
#ifdef _WIN32
	const std::string PLANETS_DIRECTORY = "D:\\SkelMUD\\SkelMUD\\SkelMUD\\Planets\\";
	const std::string PLAYERS_DIRECTORY = "D:\\SkelMUD\\SkelMUD\\SkelMUD\\Players\\";
	const std::string ACCOUNTFILE = "Accounts.dat";
#else
    const std::string PLANETS_DIRECTORY = "./Planets/";
    const std::string PLAYERS_DIRECTORY = "./Players/";
    const std::string CLASSES_DIRECTORY = "./Classes/";
    const std::string ACCOUNTFILE = "Accounts.dat";
#endif
    std::string m_filename;
    std::fstream* m_file;
};

#endif // FILE_H