#include "../includes/util.h"
#include "../includes/color.h"
#include "../includes/hittable_list.h"
#include "../includes/sphere.h"
#include "../includes/camera.h"
#include "../includes/material.h"

int main() {

    hittable_list world;

    auto material_ground = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = std::make_shared<dielectric>(1.5);
    auto material_right  = std::make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(std::make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(std::make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(std::make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(std::make_shared<sphere>(point3(-1.0,    0.0, -1.0),  -0.4, material_left));
    world.add(std::make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    camera cam;

    cam.m_aspect_ratio      = 16.0 / 9.0;
    cam.m_image_width       = 400;
    cam.m_samples_per_pixel = 100;
    cam.m_max_depth         = 50;

    cam.m_view_fov     = 20;
    cam.m_lookfrom = point3(-2,2,1);
    cam.m_lookat   = point3(0,0,-1);
    cam.m_camera_up      = vec3(0,1,0);

    cam.render(world);
}