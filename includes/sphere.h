#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable
{
public:
    point3 m_center;
    double m_radius;
    std::shared_ptr<material> m_material_ptr;

    sphere(){}
    sphere(point3 center, double radius, std::shared_ptr<material> m) :
        m_center(center), m_radius(radius), m_material_ptr(m) {};

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override
    {
        vec3 oc = r.origin() - m_center;
        auto a = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c = oc.length_squared() - m_radius * m_radius;

        auto discriminant = half_b * half_b - a*c;
        if(discriminant < 0)
        {
            return false;
        }
    
        auto sqrtd = std::sqrt(discriminant);

        // 光线与球交点
        auto root = (-half_b - sqrtd) / a;
        if(!ray_t.surrounds(root))
        {
            root = (-half_b + sqrtd) / a;
            if(!ray_t.surrounds(root))
                return false;
        }

        rec.m_t = root;
        rec.m_p = r.at(rec.m_t);
        vec3 outward_normal = (rec.m_p - m_center) / m_radius;
        rec.set_face_normal(r, outward_normal);
        rec.m_material_ptr = this->m_material_ptr;
    
        return true;
    }
};

#endif