#include "Utils.h"
#include <algorithm>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <strsafe.h>
#else

#include <dirent.h>

#endif

void Utils::RemoveEndline(std::string &data) {
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
#else

std::vector<std::string> Utils::GetFilenames(std::string path) {
    std::vector<std::string> files = std::vector<std::string>();
    DIR* dp;
    struct dirent* dirp;
    if ((dp = opendir(path.c_str())) == NULL) {
        // std::cout << "Error(" << errno << ") opening " << path << std::endl;
        // handle error
    }

    while ((dirp = readdir(dp)) != NULL) {
        std::string filename = std::string(dirp->d_name);
        if (filename == ".." or filename == ".")
            continue;
        files.push_back(path + filename);
    }
    closedir(dp);
    return files;
}

#endif

bool Utils::IsNumber(std::string entry) {
    for(int i = 0; i < entry.length(); i++)
    {
        if(!isdigit((char)entry[i])) {
            return false;
        }
    }
    return true;
}