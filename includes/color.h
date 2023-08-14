#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>

inline double linear_to_gamma(double linear_component)
{
    return std::sqrt(linear_component);
}

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel)
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // 采样次数, 求平均
    auto scale = 1.0 / samples_per_pixel;
    r = linear_to_gamma(scale * r);
    g = linear_to_gamma(scale * g);
    b = linear_to_gamma(scale * b);

    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif