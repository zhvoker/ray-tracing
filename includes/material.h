#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

struct hit_record;

class material
{
public:
    // attenuation 表示散射衰减
    // scattered   表示
    virtual bool scatter(const ray& r_in, const hit_record& rec,
        color& attenuation, ray& scattered) const = 0;
};

// 朗伯散射
class lambertian : public material
{
public:
    color m_albedo;

    lambertian(const color& a) : m_albedo(a) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec,
    color& attenuation, ray& scattered) const override
    {
        auto scatter_direction = rec.m_normal + random_unit_vector();

        if(scatter_direction.near_zero())
        {
            scatter_direction = rec.m_normal;
        }

        scattered = ray(rec.m_p, scatter_direction);
        attenuation = m_albedo;
        return true;
    }
};

class metal : public material
{
public:
    metal(const color& a, double f) : m_albedo(a), m_fuzz(f < 1 ? f : 1){}

    virtual bool scatter(const ray& r_in, const hit_record& rec,
    color& attenuation, ray& scattered) const override
    {
        // 计算光线的反射
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.m_normal);
        scattered = ray(rec.m_p, reflected + m_fuzz * random_unit_vector());
        attenuation = m_albedo;             //散射衰减
            
        // 如果反射的方向和法线是同一个半球，则表示反射成功 
        return (dot(scattered.direction(), rec.m_normal) > 0);
    }
private:
    color  m_albedo;  
    double m_fuzz;
};

class dielectric : public material
{
public:
    dielectric(double index_of_refraction) : ir(index_of_refraction){}

    bool scatter(const ray& r_in, const hit_record& rec,
    color& attenuation, ray& scattered) const override
    {
        attenuation = color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.m_is_front_face ? (1.0 / ir) : ir;

        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = std::fmin(dot(-unit_direction, rec.m_normal), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        vec3 direction;
        if(cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
        {
            direction = reflect(unit_direction, rec.m_normal);
        }
        else
        {
            direction = refract(unit_direction, rec.m_normal, refraction_ratio);
        }

        scattered = ray(rec.m_p, direction);
        return true;
    }

private:
    double ir;

    static double reflectance(double cosine, double ref_idx)
    {
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 * (1 - r0) * std::pow((1 - cosine), 5);
    }
};

#endif