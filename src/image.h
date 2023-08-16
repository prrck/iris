#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>

#include "vec3.h"

class Image {
private:
    int width;
    int height;
    std::vector<Vec3> pixels;

public:
    Image(int width, int height);
    void put_pixel(int x, int y, Vec3 color);
    void write(const std::string& filepath) const;
};

#endif // IMAGE_H
