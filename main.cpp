#include "model.hpp"
// #include "tgaimage.hpp"
#include "drawing.hpp"
#include "constants.hpp"
#include <limits>
#include <vector>
#include "gif.h"
#include <string>
#include <algorithm>

void flip_frame_vertical(std::vector<uint8_t> &frame, int width, int height)
{
    const int row_size = width * 4; // 4 bytes per pixel (RGBA)
    const int half_height = height / 2;

    for (int y = 0; y < half_height; y++)
    {
        int top_row_start = y * row_size;
        int bottom_row_start = (height - 1 - y) * row_size;

        // Swap entire rows using std::swap_ranges for efficiency
        std::swap_ranges(
            frame.begin() + top_row_start,
            frame.begin() + top_row_start + row_size,
            frame.begin() + bottom_row_start);
    }
}

int main()
{
    // TGAImage z_buffer(WIDTH, HEIGHT, TGAImage::RGB);
    std::vector<float> z_buffer(WIDTH * HEIGHT, -std::numeric_limits<float>::max());
    std::vector<uint8_t>
        frame(WIDTH * HEIGHT * 4);
    Model model("head.obj");

    const float zoom = 2;

    // for (int i = 0; i < 3; i++)
    // {
    //     draw_model(model, zoom, 3.1415f / 3 * i, WHITE, frame, z_buffer);
    //     TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);
    //     for (int p = 0; p < WIDTH * HEIGHT * 4; p += 4)
    //     {
    //         int pixel_index = p / 4;
    //         int x = pixel_index % WIDTH;
    //         int y = pixel_index / WIDTH;

    //         // Frame data is in RGBA format, TGAColor expects BGRA
    //         TGAColor color;
    //         color[2] = frame[p];     // R -> B position
    //         color[1] = frame[p + 1]; // G -> G position
    //         color[0] = frame[p + 2]; // B -> R position
    //         color[3] = frame[p + 3]; // A -> A position

    //         image.set(x, y, color);
    //     }
    //     image.write_tga_file(std::to_string(i) + "output.tga");
    //     std::fill(frame.begin(), frame.end(), 0);
    //     std::fill(z_buffer.begin(), z_buffer.end(), 0);
    // }

    const int nframes = 200;
    const int delay = 1000 / nframes;
    GifWriter g;
    GifBegin(&g, "animation.gif", WIDTH, HEIGHT, delay);

    for (int i = 0; i < nframes; i++)
    {
        draw_model(model, zoom, 2 * 3.1415f / nframes * i, WHITE, frame, z_buffer);
        flip_frame_vertical(frame, WIDTH, HEIGHT);
        GifWriteFrame(&g, frame.data(), WIDTH, HEIGHT, delay);
        std::fill(frame.begin(), frame.end(), 0);
        std::fill(z_buffer.begin(), z_buffer.end(), 0);
        Log("Frame: " + std::to_string(i + 1) + "/" + std::to_string(nframes))
    }

    GifEnd(&g);
}
