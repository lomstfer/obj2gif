#include "model.hpp"
#include "tgaimage.hpp"
#include "drawing.hpp"
#include "constants.hpp"
#include <limits>
#include <vector>

int main()
{
    TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);

    Model model("cup.obj");
    const float zoom = 2;
    const Vec3f light_dir = Vec3f(0, 0, -1);

    std::vector<float> zbuffer(HEIGHT * WIDTH);

    draw_model(model, zoom, WHITE, image, light_dir, zbuffer);

    // Vec2i t0[3] = {Vec2i(7, 45), Vec2i(35, 100), Vec2i(45, 60)};
    // Vec2i t1[3] = {Vec2i(120, 35), Vec2i(90, 5), Vec2i(45, 110)};
    // Vec2i t2[3] = {Vec2i(115, 83), Vec2i(80, 90), Vec2i(85, 120)};
    // draw_triangle_no_z_buffer(t0[0], t0[1], t0[2], RED, image);
    // draw_triangle_no_z_buffer(t1[0], t1[1], t1[2], WHITE, image);
    // draw_triangle_no_z_buffer(t2[0], t2[1], t2[2], GREEN, image);

    // draw_triangle_no_z_buffer(Vec2i(1, 1), Vec2i(5, 5), Vec2i(5, 1), RED, image);

    image.write_tga_file("output.tga");
}
