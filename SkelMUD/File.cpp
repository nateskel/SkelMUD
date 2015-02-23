#include "File.h"
#include "Utils.h"
#include <iostream>
#include <vector>

File::File()
{
	
}

File::~File()
{

}

void File::OpenRead(std::string filename)
{
	m_file.open(filename.c_str(), std::ios::in);
}

void File::OpenWrite(std::string filename)
{
	m_file.open(filename.c_str(), std::ios::out);
}

void File::OpenReadWrite(std::string filename)
{
	m_file.open(filename.c_str(), std::ios::out | std::ios::in);
}

void File::Close()
{
	m_file.close();
	m_file.flush();
}

bool File::ReadNextLine(std::string &data)
{
	std::getline(m_file, data);
	Utils::RemoveEndline(data);
	if (m_file.good())
	{
		return true;
	}
	else
		return false;
}

void File::WriteLine(std::string data)
{
	m_file << data << "\r\n";
}

std::vector<Connection::Account> File::LoadAccounts()
{
	Connection::Account account;
	std::vector<Connection::Account> accounts;
	std::string data;
	OpenRead("Accounts.dat");
	while (ReadNextLine(data))
	{
		account.username = data;
		if (!ReadNextLine(data))
			return accounts;
		account.password = data;
		if (!ReadNextLine(data))
			return accounts;
		account.id = std::stoi(data);
	}
	accounts.push_back(account);
	return accounts;
	Close();
}

std::vector<Planet*> File::LoadPlanets()
{
	std::vector<Planet*> planets;
	std::string data;
	std::vector<std::string> files = Utils::GetFilenames(DIRECTORY);
	for (int i = 0; i < files.size(); i++)
	{
		Planet* planet = new Planet();
		OpenRead(files[i]);
		ReadNextLine(data);
		planet->SetName(data);
		ReadNextLine(data);
		planet->coordinates.x = std::atoi(data.c_str());
		ReadNextLine(data);
		planet->coordinates.y = std::atoi(data.c_str());
		ReadNextLine(data);
		planet->coordinates.z = std::atoi(data.c_str());
		while (ReadNextLine(data))
		{
			int id = std::atoi(data.c_str());
			ReadNextLine(data);
			ReadNextLine(data);
			int n = std::atoi(data.c_str());
			ReadNextLine(data);
			int s = std::atoi(data.c_str());
			ReadNextLine(data);
			int e = std::atoi(data.c_str());
			ReadNextLine(data);
			int w = std::atoi(data.c_str());
			ReadNextLine(data);
			int ne = std::atoi(data.c_str());
			ReadNextLine(data);
			int nw = std::atoi(data.c_str());
			ReadNextLine(data);
			int se = std::atoi(data.c_str());
			ReadNextLine(data);
			int sw = std::atoi(data.c_str());
			ReadNextLine(data);
			int u = std::atoi(data.c_str());
			ReadNextLine(data);
			int d = std::atoi(data.c_str());
			ReadNextLine(data);
			int timed = std::atoi(data.c_str());
			ReadNextLine(data);
			int time = std::atoi(data.c_str());
			ReadNextLine(data);
			std::string timedmessage = data;
			ReadNextLine(data);
			int commanded = std::atoi(data.c_str());
			ReadNextLine(data);
			std::string command = data;
			ReadNextLine(data);
			std::string commandmessage = data;
			ReadNextLine(data);
			int zone = std::atoi(data.c_str());
			ReadNextLine(data);
			std::string shortdescription = data;
			std::string longdescription = "";
			ReadNextLine(data);
			while (data != "@End")
			{
				longdescription.append(data);
				longdescription.append("\r\n");
				ReadNextLine(data);
			}
			planet->AddRoom(new Room(longdescription, shortdescription, n, s, e, w, ne, nw, se, sw, u, d));
		}
		planets.push_back(planet);
		Close();
	}
	return planets;
}