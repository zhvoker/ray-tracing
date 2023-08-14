#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <limits>
#include <cstdlib>
#include <memory>

const double infinity =  std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// 将角度转换为弧度
inline double degrees_to_radians(double degress)
{
    return degress * pi / 180.0;
}

inline double clamp(double x, double min, double max)
{
    if(x < min) return min;
    if(x > max) return max;
    return x;
}

// 随机数构造
inline double random_double()
{
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max)
{
    return min + (max - min) * random_double();
}

inline int random_int(int min, int max)
{
    return static_cast<int>(random_double(min, max+1));
}

#endif