#include "Utils.h"
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#include <strsafe.h>
#endif

void Utils::RemoveEndline(std::string &data)
{
	data.erase(std::remove(data.begin(), data.end(), '\n'), data.end());
	data.erase(std::remove(data.begin(), data.end(), '\r'), data.end());
}

#ifdef _WIN32
std::vector<std::string> Utils::GetFilenames(std::string path)
{
	std::vector<std::string> output;
	TCHAR szDIR[MAX_PATH];
	StringCchCopy(szDIR, MAX_PATH, path.c_str());
	StringCchCat(szDIR, MAX_PATH, "*");
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA fd;
	hFind = FindFirstFile(szDIR, &fd);
	FindNextFile(hFind, &fd);
	while (FindNextFile(hFind, &fd))
	{
		std::string filename = path;
		filename.append(std::string(fd.cFileName));
		output.push_back(filename);
	}
	FindClose(hFind);
	return output;
}
#endif