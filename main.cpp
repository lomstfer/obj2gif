#include "model.hpp"
#include "tgaimage.hpp"
#include "drawing.hpp"
#include "constants.hpp"
#include <limits>
#include <vector>

int main()
{
    TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);
    TGAImage z_buffer(WIDTH, HEIGHT, TGAImage::RGB);

    Model model("cup.obj");
    const float zoom = 2;

    draw_model(model, zoom, WHITE, image, z_buffer);

    // draw_triangle(Vec3i(0, 255, 255), Vec3i(100, 100, 0), Vec3i(200, 20, 255), WHITE, image);

    image.write_tga_file("output.tga");
    z_buffer.write_tga_file("z_buffer.tga");
}
