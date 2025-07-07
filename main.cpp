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

int main(int argc, char *argv[])
{
    std::vector<float> z_buffer(WIDTH * HEIGHT, -std::numeric_limits<float>::max());
    std::vector<uint8_t>
        frame(WIDTH * HEIGHT * 4);

    std::string model_file = "test.obj";
    if (argc > 1)
    {
        model_file = argv[1];
    }
    else if (!_DEBUG)
    {
        Log("usage: obj2gif [filename]");
        return 0;
    }

    Model model(model_file);

    const int nframes = 200;
    const int delay = std::max(2, 500 / nframes);
    GifWriter g;
    GifBegin(&g, "animation.gif", WIDTH, HEIGHT, delay);

    for (int i = 0; i < nframes; i++)
    {
        draw_model(model, 2 * 3.1415f / nframes * i, Color{0, 255, 255, 255}, frame, z_buffer);
        flip_frame_vertical(frame, WIDTH, HEIGHT);

        GifWriteFrame(&g, frame.data(), WIDTH, HEIGHT, delay);

        std::fill(frame.begin(), frame.end(), 0);
        std::fill(z_buffer.begin(), z_buffer.end(), 0);
        Log("Frame: " + std::to_string(i + 1) + "/" + std::to_string(nframes));
    }

    GifEnd(&g);
}
