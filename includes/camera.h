#ifndef  CAMERA_H
#define  CAMERA_H

#include <iostream>

#include "util.h"
#include "color.h"
#include "hittable.h"
#include "material.h"

class camera
{
public:
    void render(const hittable& world)
    {
        initialize();

        std::cout << "P3\n" << m_image_width << ' ' << m_image_height << "\n255\n";

        for (int j = 0; j < m_image_height; ++j) {
            std::clog << "\rScanlines remaining: " << (m_image_height - j) << ' ' << std::flush;
            for (int i = 0; i < m_image_width; ++i) {
                color pixel_color(0,0,0);
                for (int sample = 0; sample < m_samples_per_pixel; ++sample) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, m_max_depth, world);
                }
                write_color(std::cout, pixel_color, m_samples_per_pixel);
            }
        }
        std::clog << "\rDone.              \n";
    }

private:
    void initialize()
    {
        m_image_height = static_cast<int>(m_image_width / m_aspect_ratio);
        m_image_height = (m_image_height < 1) ? 1 : m_image_height;

        m_camera_center = m_lookfrom;

        // Determine viewport dimensions.
        auto theta = degrees_to_radians(m_view_fov);
        auto h = std::tan(theta/2);
        auto viewport_height = 2 * h * m_focus_dist;
        auto viewport_width = viewport_height * (static_cast<double>(m_image_width) / m_image_height);

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
        m_w = unit_vector(m_lookfrom - m_lookat);
        m_u = unit_vector(cross(m_camera_up, m_w));
        m_v = cross(m_w, m_u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        vec3 viewport_u = viewport_width * m_u;    // Vector across viewport horizontal edge
        vec3 viewport_v = viewport_height * -m_v;  // Vector down viewport vertical edge

        // Calculate the horizontal and vertical delta vectors to the next pixel.
        m_pixel_delta_u = viewport_u / m_image_width;
        m_pixel_delta_v = viewport_v / m_image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = m_camera_center - (m_focus_dist * m_w) - viewport_u/2 - viewport_v/2;
        m_pixel_00_location = viewport_upper_left + 0.5 * (m_pixel_delta_u + m_pixel_delta_v);

        // Calculate the camera defocus disk basis vectors.
        auto defocus_radius = m_focus_dist * std::tan(degrees_to_radians(m_defocus_angle / 2));
        m_defocus_disk_u = defocus_radius * m_u;
        m_defocus_disk_v = defocus_radius * m_v;
    }

    ray get_ray(int i, int j) const 
    {
        auto pixel_center = m_pixel_00_location + (i * m_pixel_delta_u) + (j * m_pixel_delta_v);
        auto pixel_sample = pixel_center + pixel_sample_square();

        auto ray_origin = (m_defocus_angle <= 0) ? m_camera_center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 pixel_sample_square() const {
        // Returns a random point in the square surrounding a pixel at the origin.
        auto px = -0.5 + random_double();
        auto py = -0.5 + random_double();
        return (px * m_pixel_delta_u) + (py * m_pixel_delta_v);
    }

    vec3 pixel_sample_disk(double radius) const {
        // Generate a sample from the disk of given radius around a pixel at the origin.
        auto p = radius * random_in_unit_disk();
        return (p[0] * m_pixel_delta_u) + (p[1] * m_pixel_delta_v);
    }

    point3 defocus_disk_sample() const {
        // Returns a random point in the camera defocus disk.
        auto p = random_in_unit_disk();
        return m_camera_center + (p[0] * m_defocus_disk_u) + (p[1] * m_defocus_disk_v);
    }

    color ray_color(const ray& r, int depth, const hittable& world) const {
        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0)
            return color(0,0,0);

        hit_record rec;

        if (world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            color attenuation;
            if (rec.m_material_ptr->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth-1, world);
            return color(0,0,0);
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5*(unit_direction.y() + 1.0);
        return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
    }

public:
    double m_aspect_ratio   = 1.0;
    int m_image_width       = 100;
    int m_samples_per_pixel = 10;
    int m_max_depth         = 10;

    double m_view_fov       = 90;
    point3 m_lookfrom       = point3(0, 0, -1);
    point3 m_lookat         = point3(0, 0, 0);
    vec3   m_camera_up      =   vec3(0, 1, 0);

    double m_defocus_angle  = 0;
    double m_focus_dist     = 10;

private:
    int m_image_height;
    point3 m_camera_center;
    point3 m_pixel_00_location;
    vec3 m_pixel_delta_u;
    vec3 m_pixel_delta_v;
    vec3 m_u, m_v, m_w;
    vec3 m_defocus_disk_u;
    vec3 m_defocus_disk_v;
};

#endif