#ifndef RAYTRACE_H
#define RAYTRACE_H

#include<cmath>
#include<cstdlib>
#include<limits>
#include<memory>
using namespace std;

const double infinity = numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;
inline double degree_to_radians(double degrees){
    return degrees * pi /180.0;
}

#include "vec3.h"
#include "ray.h"
#endif