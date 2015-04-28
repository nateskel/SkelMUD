#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

class Utils {
public:
    static void RemoveEndline(std::string &data);

    static std::vector<std::string> GetFilenames(std::string path);

protected:
private:
};

#endif // UTILS_H