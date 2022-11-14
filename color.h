#ifndef COLOR_H
#define COLOR_H
using namespace std;
#include "vec3.h"
#include <iostream>

void write_color(std::ostream &out, color pixel_color,int sample_per_pixel) {
    auto scale = 1.0/sample_per_pixel;
    auto r = pow(pixel_color.x()*scale,1.0/2.2);
    auto g = pow(pixel_color.y()*scale,1.0/2.2);
    auto b = pow(pixel_color.z()*scale,1.0/2.2);
    if (r != r) r = 0.0;
    if (g != g) g = 0.0;
    if (b != b) b = 0.0;
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r,0.0,0.999)) << ' '
        << static_cast<int>(256 * clamp(g,0.0,0.999)) << ' '
        << static_cast<int>(256 * clamp(b,0.0,0.999)) << '\n';
}

#endif