#ifndef CAMERA_H
#define CAMERA_H

#include "random.h"
#include "ray.h"
#include "vec3.h"

class Camera {
public:
    Camera(Vec3 from, Vec3 to, int width, int height, float focal);
    Ray generate_primary_ray(float x, float y, bool jitter = true) const;

private:
    Vec3 position;
    Vec3 forward;
    Vec3 up;
    Vec3 right;
    Vec3 screen_center;
    float focal;
    float ratio;
    float pixel_size;
};

#endif // CAMERA_H
