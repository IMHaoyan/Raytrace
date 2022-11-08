#ifndef HEAD_H
#define HEAD_H

#include "Raytrace.h"
#include "color.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include "texture.h"
#include <iostream>
#include "material.h"
#include "bvh.h"
#include<iomanip>
#include <thread>
#include <mutex>

string timeToString(std::chrono::system_clock::time_point &t)
{
    std::time_t time = std::chrono::system_clock::to_time_t(t);
    std::string time_str = std::ctime(&time);
    time_str.resize(time_str.size() - 1);
    return time_str;
}
#endif