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

void File::OpenRead(std::fstream& stream, std::string filename)
{
	//m_file->open(filename.c_str(), std::ios::in);
	stream.open(filename.c_str(), std::ios::in);
}

void File::OpenWrite(std::string filename)
{
	m_file->open(filename.c_str(), std::ios::out);
}

void File::OpenReadWrite(std::string filename)
{
	m_file->open(filename.c_str(), std::ios::out | std::ios::in);
}

void File::Close(std::fstream& stream)
{
	stream.close();
	stream.flush();
}

bool File::ReadNextLine(std::fstream &stream, std::string &data)
{
	std::getline(stream, data);
	Utils::RemoveEndline(data);
	return stream.good();
}

void File::WriteLine(std::string data)
{
	*m_file << data << "\r\n";
}

std::vector<Connection::Account> File::LoadAccounts()
{
	Connection::Account account;
	std::vector<Connection::Account> accounts;
	std::string data;
	std::fstream stream;
	OpenRead(stream, "Accounts.dat");

	while (ReadNextLine(stream, data))
	{
		account.username = data;
		if (!ReadNextLine(stream, data))
			return accounts;
		account.password = data;
		if (!ReadNextLine(stream, data))
			return accounts;
		account.id = std::stoi(data);
	}
	accounts.push_back(account);
	Close(stream);
	return accounts;
}

std::vector<Planet*> File::LoadPlanets()
{
	std::vector<Planet*> planets = std::vector<Planet*>();
	std::string data;
	std::vector<std::string> files = Utils::GetFilenames(DIRECTORY);
	std::fstream stream;
	for (int i = 0; i < files.size(); i++)
	{
		if(files[i] == ".." || files[i] == ".")
			continue;
		Planet* planet = new Planet();
		OpenRead(stream, files[i]);
		ReadNextLine(stream, data);
		planet->SetName(data);
		ReadNextLine(stream, data);
		planet->coordinates.x = std::atoi(data.c_str());
		ReadNextLine(stream, data);
		planet->coordinates.y = std::atoi(data.c_str());
		ReadNextLine(stream, data);
		planet->coordinates.z = std::atoi(data.c_str());
		while (ReadNextLine(stream, data))
		{
			int id = std::atoi(data.c_str());
			ReadNextLine(stream, data);
			ReadNextLine(stream, data);
			int n = std::atoi(data.c_str());
			ReadNextLine(stream, data);
			int s = std::atoi(data.c_str());
			ReadNextLine(stream, data);
			int e = std::atoi(data.c_str());
			ReadNextLine(stream, data);
			int w = std::atoi(data.c_str());
			ReadNextLine(stream, data);
			int ne = std::atoi(data.c_str());
			ReadNextLine(stream, data);
			int nw = std::atoi(data.c_str());
			ReadNextLine(stream, data);
			int se = std::atoi(data.c_str());
			ReadNextLine(stream, data);
			int sw = std::atoi(data.c_str());
			ReadNextLine(stream, data);
			int u = std::atoi(data.c_str());
			ReadNextLine(stream, data);
			int d = std::atoi(data.c_str());
			ReadNextLine(stream, data);
			int timed = std::atoi(data.c_str());
			ReadNextLine(stream, data);
			int time = std::atoi(data.c_str());
			ReadNextLine(stream, data);
			std::string timedmessage = data;
			ReadNextLine(stream, data);
			int commanded = std::atoi(data.c_str());
			ReadNextLine(stream, data);
			std::string command = data;
			ReadNextLine(stream, data);
			std::string commandmessage = data;
			ReadNextLine(stream, data);
			int zone = std::atoi(data.c_str());
			ReadNextLine(stream, data);
			std::string shortdescription = data;
			std::string longdescription = "";
			ReadNextLine(stream, data);
			while (data != "@End")
			{
				longdescription.append(data);
				longdescription.append("\r\n");
				ReadNextLine(stream, data);
			}
			planet->AddRoom(new Room(longdescription, shortdescription, n, s, e, w, ne, nw, se, sw, u, d));
		}
		planets.push_back(planet);
		Close(stream);
	}
	return planets;
}