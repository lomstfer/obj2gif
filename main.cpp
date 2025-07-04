#include "model.hpp"
#include "tgaimage.hpp"
#include "drawing.hpp"
#include "constants.hpp"
#include <limits>
#include <vector>

int main()
{
    TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);
    // TGAImage z_buffer(WIDTH, HEIGHT, TGAImage::RGB);
    std::vector<float> z_buffer(WIDTH * HEIGHT);

    Model model("evil.obj");
    const float zoom = 2;
    
    draw_model(model, zoom, WHITE, image, z_buffer);
    
    Log("hej")
    image.write_tga_file("output.tga");
}
