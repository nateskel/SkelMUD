#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <map>
#include <algorithm>

class Utils {
public:

    struct Vector3 {
        double x;
        double y;
        double z;
    };


    static void RemoveEndline(std::string &data);

    static std::vector<std::string> GetFilenames(std::string path);

    static bool IsNumber(std::string entry);

    static double GetDistance(Vector3 origin, Vector3 destination);

    static double GetDistance(double ox, double oy, double oz, double dx, double dy, double dz);

    static double GetDistance(double ox, double oy, double oz, Vector3 destination);

    static int make_directory(const char *path, mode_t mode);

    static std::string FindMatch(std::vector<std::string> list, std::string value);

    template <typename T, typename T2>
    static std::vector<T> ExtractMapKeys(std::map<T, T2> input) {
        std::vector<T> output;
        for (auto const &item: input) {
            output.push_back(item.first);
        }
        return output;
    }

    template <typename T, typename T2>
    static std::vector<T2> ExtractMapValues(std::map<T, T2> input) {
        std::vector<T2> output;
        for (auto const &item: input) {
            output.push_back(item.second);
        }
        return output;
    }

protected:
private:
};

#endif // UTILS_H