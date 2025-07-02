#include "model.hpp"
#include "tgaimage.hpp"
#include "drawing.hpp"

const TGAColor white{255, 255, 255, 255};
const TGAColor red{0, 0, 255, 255};
const TGAColor green{255, 0, 0, 255};

const int width = 900;
const int height = 900;


int main()
{
    TGAImage image(width, height, TGAImage::RGB);
    
    Model model("cup.obj");
    const float zoom = 2;
    const Vec3f light_dir = Vec3f(0, 0, -1);

    draw_model(model, zoom, white, image, light_dir);

    image.write_tga_file("output.tga");
}
