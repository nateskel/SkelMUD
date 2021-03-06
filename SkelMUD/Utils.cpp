#include "Utils.h"
#include "Tokenizer.h"
#include <algorithm>
#include <iostream>
#include <math.h>

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
    if(entry.length() == 0)
        return false;
    if(entry[0] != '-' and !isdigit((char)entry[0]))
        return false;
    for(int i = 1; i < entry.length(); i++)
    {
        if(!isdigit((char)entry[i])) {
            return false;
        }
    }
    return true;
}

double Utils::GetDistance(Utils::Vector3 origin, Utils::Vector3 destination) {
    double x = destination.x - origin.x;
    double y = destination.y - origin.y;
    double z = destination.z - origin.z;
    return std::sqrt(x * x + y * y + z * z);
}

double Utils::GetDistance(double ox, double oy, double oz, double dx, double dy, double dz) {
    double x = dx - ox;
    double y = dy - oy;
    double z = dz - oz;
    return std::sqrt(x * x + y * y + z * z);
}

double Utils::GetDistance(double ox, double oy, double oz, Vector3 destination) {
    double x = destination.x - ox;
    double y = destination.y - oy;
    double z = destination.z - oz;
    return std::sqrt(x * x + y * y + z * z);
}

int Utils::make_directory(const char *path, mode_t mode) {
    struct stat st;
    int status = 0;
    if (stat(path, &st) != 0)
    {
        if (mkdir(path, mode) != 0 && errno != EEXIST)
            status = -1;
    }
    else if (!S_ISDIR(st.st_mode))
    {
        errno = ENOTDIR;
        status = -1;
    }

    return(status);
}

std::string Utils::FindMatch(std::vector<std::string> list, std::string value) {
    for(auto item: list) {
        std::string check_string = item;
        while(check_string.length() > 0) {
            if(check_string.substr(0, value.length()) == value)
                return item;
            else
                Tokenizer::GetFirstToken(check_string, true);
        }
    }
    return value;
}

//template <class T>
//std::vector<std::string> Utils::ExtractMapKeys(std::map<std::string, T> input) {
//    std::vector<std::string> output;
//    for(auto const& item: input) {
//        output.push_back(item.first);
//    }
//    std::sort(output.begin(), output.end());
//    return output;
//}
