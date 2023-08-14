#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3
{
public:
    double e[3];
        
    vec3() : e{0, 0, 0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const {return e[0];}
    double y() const {return e[1];}
    double z() const {return e[2];}

    vec3 operator-() const {return vec3(-e[0], -e[1], -e[2]);}
    double operator[] (int i) const {return e[i];}
    double& operator[] (int i) {return e[i];}

    vec3& operator+=(const vec3 &v)
        {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

    vec3& operator*=(const double t)
        {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

    vec3& operator/=(const double t)
        {
            return *this *= 1/t;
        }

    double length() const
        {
            return sqrt(length_squared());
        }

    double length_squared() const
        {
            return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
        }

    bool near_zero() const
        {
            const auto s = 1e-8;
            return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
        }

    inline static vec3 random()
    {
        return vec3(random_double(), random_double(), random_double());
    }

    inline static vec3 random(double min, double max)
    {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }
};

using point3 = vec3;
using color = vec3;

inline std::ostream& operator<<(std::ostream &out, const vec3 &v)
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(const double t, const vec3 &v)
{
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3 &v, const double t)
{
    return t * v;
}

inline vec3 operator/(const vec3 &u, double t)
{
    return (1/t) * u;
}

// 点积
inline double dot(const vec3 &u, const vec3 &v)
{
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

// 叉积
inline vec3 cross(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

// 向量单位化
inline vec3 unit_vector(vec3 v)
{
    return v / v.length();
}

// 模拟透镜, 在透镜上随机模拟一点
inline vec3 random_in_unit_disk()
{
    while(true)
    {
        auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if(p.length_squared() < 1)
        {
            return p;
        }
    }
}

inline vec3 random_in_unit_sphere()
{
    while(true)
    {
        // 生成单位球体的一个随机向量，x,y,z分量都是从-1到1的随机数
        auto p = vec3::random(-1, 1);
        // 如果随机向量的长度>=1表示在单位球体之外，则重新随机生成
        if(p.length_squared() >= 1)
            continue;
        return p;
    }
}

inline vec3 random_unit_vector()
{
    return unit_vector(random_in_unit_sphere());
}

inline vec3 reflect(const vec3& v, const vec3& n)
{
    return v - 2 * dot(v, n) * n;
}

inline vec3 refract(const vec3& ray_in, const vec3& normal, double etai_over_etat)
{
    auto cos_theta = std::fmin(dot(-ray_in, normal), 1.0);
    vec3 ray_out_perp = etai_over_etat * (ray_in + cos_theta * normal);
    vec3 ray_out_parallel = -std::sqrt(std::fabs(1.0 - ray_out_perp.length_squared())) * normal;
    return ray_out_perp + ray_out_parallel;
}

#endif