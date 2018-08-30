#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>


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

protected:
private:
};

#endif // UTILS_H