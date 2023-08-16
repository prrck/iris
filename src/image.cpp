#include "image.h"

#include <ImfArray.h>
#include <ImfRgbaFile.h>
#include <iostream>

Image::Image(int width, int height)
    : width(width)
    , height(height)
    , pixels(width * height)
{
}

void Image::put_pixel(int x, int y, Vec3 color)
{
    int pixel_id = (y * width + x);

    pixels[pixel_id] = color;
}

void Image::write(const std::string& filepath) const
{
    // TODO: Optimize: Copy array to array useless.
    Imf::Array2D<Imf::Rgba> pixels_buffer(width, height);
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++) {
            int pixel_id = (x * height + y);
            pixels_buffer[x][y] = Imf::Rgba(
                pixels[pixel_id].x,
                pixels[pixel_id].y,
                pixels[pixel_id].z);
        }

    try {
        Imf::RgbaOutputFile file(filepath.c_str(), width, height, Imf::WRITE_RGBA);
        file.setFrameBuffer(&pixels_buffer[0][0], 1, width);
        file.writePixels(height);
    } catch (const std::exception& e) {
        std::cerr << "error writing image file " << filepath << ": " << e.what() << std::endl;
    }
}