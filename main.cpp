#include "model.hpp"
#include "drawing.hpp"
#include "constants.hpp"
#include <limits>
#include <vector>
#include "gif.h"
#include <string>
#include <algorithm>

void flip_frame_vertical(std::vector<uint8_t> &frame, int width, int height)
{
    const int row_size = width * 4;
    const int half_height = height / 2;

    for (int y = 0; y < half_height; y++)
    {
        int top_row_start = y * row_size;
        int bottom_row_start = (height - 1 - y) * row_size;

        std::swap_ranges(
            frame.begin() + top_row_start,
            frame.begin() + top_row_start + row_size,
            frame.begin() + bottom_row_start);
    }
}

int main()
{
    std::vector<float> z_buffer(WIDTH * HEIGHT, -std::numeric_limits<float>::max());
    std::vector<uint8_t>
        frame(WIDTH * HEIGHT * 4);
    Model model("head.obj");

    const float zoom = 2;

    const int nframes = 700;
    const int delay = 2;
    GifWriter g;
    GifBegin(&g, "animation.gif", WIDTH, HEIGHT, delay);

    for (int i = 0; i < nframes; i++)
    {
        draw_model(model, zoom, 2 * 3.1415f / nframes * i, Color{0, 255, 255, 255}, frame, z_buffer);
        flip_frame_vertical(frame, WIDTH, HEIGHT);

        GifWriteFrame(&g, frame.data(), WIDTH, HEIGHT, delay);

        std::fill(frame.begin(), frame.end(), 0);
        std::fill(z_buffer.begin(), z_buffer.end(), 0);
        Log("Frame: " + std::to_string(i + 1) + "/" + std::to_string(nframes));
    }

    GifEnd(&g);
}
