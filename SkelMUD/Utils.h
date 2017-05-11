#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

class Utils {
public:
    static void RemoveEndline(std::string &data);

    static std::vector<std::string> GetFilenames(std::string path);

    static bool IsNumber(std::string entry);

    struct Coordinates {
        double x;
        double y;
        double z;
    };

    struct Velocity {
        double x;
        double y;
        double z;
    };

protected:
private:
};

#endif // UTILS_H