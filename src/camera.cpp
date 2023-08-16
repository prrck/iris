#include "camera.h"

Camera::Camera(Vec3 from, Vec3 to, int width, int height, float focal)
{
    ratio = float(height) / float(width);
    pixel_size = 1.0 / float(width);
    this->focal = focal;

    position = from;
    forward = (to - from).normalize();
    right = forward.cross(Vec3(0.0, 1.0, 0.0));
    up = right.cross(forward);

    screen_center = position + forward * focal;
}

Ray Camera::generate_primary_ray(float x, float y, bool jitter) const
{

    float screen_x = x - 0.5;
    float screen_y = y - 0.5;

    if (jitter) {
        screen_x += rand_uniform_float() * pixel_size;
        screen_y += rand_uniform_float() * pixel_size;
    }

    Vec3 world_pos = screen_center + right * screen_x + up * ratio * screen_y;

    return Ray(position, (world_pos - position).normalize());
}