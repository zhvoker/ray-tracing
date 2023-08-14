#ifndef HITTABLE_H
#define HITTABLE_H

#include "interval.h"
#include "ray.h"

class material;

struct hit_record
{
    point3 m_p;
    vec3 m_normal;
    std::shared_ptr<material> m_material_ptr;
    double m_t;
    bool m_is_front_face;

    inline void set_face_normal(const ray& r, const vec3& outward_normal)
    {
        m_is_front_face = dot(r.direction(), outward_normal) < 0;
        m_normal = m_is_front_face ? outward_normal : -outward_normal;
    }

};

class hittable
{
public:
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif