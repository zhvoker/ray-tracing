#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray
{
public:

    // 成员变量
    point3 m_origin;
    vec3 m_dir;

    // 构造函数
    ray() {}
    ray(const point3& origin, const vec3& direction)
        : m_origin(origin), m_dir(direction) {}
    ~ray(){}

    point3 origin() const {return m_origin;}
    vec3 direction() const {return m_dir;}
        
    point3 at(double t) const
    {
        return m_origin + t * m_dir;
    }
};

#endif